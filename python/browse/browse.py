import webbrowser
import re
import sys
import argparse
import codecs
from urllib.parse import urlparse
from os import curdir, listdir, remove, chdir
from os.path import splitext, dirname, join, abspath, exists
from http.server import BaseHTTPRequestHandler, HTTPServer
from shutil import copyfile, rmtree

# Tremppi server that communicates between HTML reports and the filesystem
class StoreHandler(BaseHTTPRequestHandler):
    # return current files with the .html suffix
    def get_files(self):
        files = listdir(".")
        files = [value for value in files if re.match("^((?!browse).)*html", value)]
        print(files)
        return "\n".join(files)

    # obtain content files (standard browsing)
    def provide_content(self, store_path):
        file_name, file_extension = splitext(store_path)
        file_extension = file_extension[1:]
        if file_extension == "js":
            mime_type = "text/javascript"
        elif file_extension == "png":
            mime_type = "image/png"
        elif file_extension == "ico":
            mime_type = "image/x-icon"
            self.success_response(mime_type, codecs.open(store_path, encoding="base64").read())
            return
        else:
            mime_type = "text/" + file_extension
        with open(store_path) as fh:
            self.success_response(mime_type, fh.read().encode())

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
                store_path = "." + parsed_path.path
                self.provide_content(store_path)
        else:
            # get list of HTML files in the topmost directory
            if parsed_path.query == "files":
                self.success_response('text/plain', self.get_files().encode())
            # read content of a file under given url - used for loading models
            if parsed_path.query == "content":
                store_path = join(curdir, parsed_path.query)
                with open(store_path) as fh:
                    self.success_response('text/js', fh.read().encode())
            # obtain columns of the database
            elif parsed_path.query == "columns":
                self.success_response('text/plain', "obtain columns".encode())


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