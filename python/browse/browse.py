import webbrowser
import sys
import argparse
import codecs
from urllib.parse import urlparse
from os import curdir, listdir, remove, chdir
from os.path import splitext, dirname, join, abspath, exists
from http.server import BaseHTTPRequestHandler, HTTPServer, SimpleHTTPRequestHandler
import base64
import content_functions

# Tremppi server that communicates between HTML reports and the filesystem
class StoreHandler(SimpleHTTPRequestHandler):
    def success_response(self, type, data):
        self.send_response(200)
        self.send_header('Content-type', type)
        self.end_headers()
        self.wfile.write(data)

    # respond to the GET request
    def do_GET(self):
        parsed_path = urlparse(self.path)
        print(parsed_path.path)
        if parsed_path.query == "":
            if parsed_path.path == "/":
                self.success_response('text/plain', "tremmpi browse is running".encode())
            else:
                return SimpleHTTPRequestHandler.do_GET(self)
        else:
            # get list of HTML files in the topmost directory
            if parsed_path.query == "files":
                self.success_response('text/plain', content_functions.get_files().encode())
            # read content of a file under given url - used for loading models
            if parsed_path.query == "content":
                store_path = join(curdir, parsed_path.query)
                with open(store_path) as fh:
                    self.success_response('text/js', fh.read().encode())
            # obtain columns of the database
            elif parsed_path.query == "columns":
                if not exists('database.sqlite'): # send no content if the database is missing
                    self.send_response(204)
                else:
                    self.success_response('text/plain', content_functions.get_columns_names().encode())
            # obtain database rows
            elif parsed_path.query == "rows":
                self.success_response('text/plain', content_functions.get_rows().encode())



    # respond to the post request
    # writes the content of the message to the file specified by the URL
    def do_POST(self):
        length = self.headers['content-length']
        data = self.rfile.read(int(length))
        parsed_path = urlparse(self.path)
        store_path = join(curdir, parsed_path.query)
        with open(store_path, 'w') as fh:
            fh.write(data.decode())
            self.send_header('Content-type', 'text')
            self.end_headers()
            self.wfile.write("".encode())
        self.send_response(200)

# options and system setup
parser = argparse.ArgumentParser(description='Initiate a TREMPPI project.')
parser.add_argument('--dest', help='specify the browsing location.')
args = parser.parse_args()
sys.path.append(dirname(dirname(abspath(sys.argv[0]))))
from tremppi_common.file_manipulation import copyanything, normal_paths

EXEC_PATH, BIN_PATH, HOME_PATH, DEST_PATH = normal_paths(sys.argv[0], args)

# start the server and open the webpage
chdir(DEST_PATH)
server = HTTPServer(('', 8080), StoreHandler)
webbrowser.open("http://localhost:8080/browse.html")
server.serve_forever()