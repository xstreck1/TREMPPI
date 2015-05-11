import webbrowser
import sys
import argparse
import json
from urllib.parse import urlparse, parse_qs
from os import curdir, chdir
from os.path import dirname, join, abspath, exists, normpath, isfile
from http.server import  HTTPServer, SimpleHTTPRequestHandler
sys.path.append(dirname(dirname(abspath(sys.argv[0]))))
from tremppi.file_manipulation import replace_regex, normal_paths
from tremppi.header import widgets, data_folder, default_port
from configure.configure import generateFiles

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
        # print(parsed_path.path)
        if parsed_path.query == "" or parsed_path.query[0] == "_":
            if parsed_path.path == "/":
                self.success_response('text/plain', "tremmpi browse is running".encode())
            else:
                return SimpleHTTPRequestHandler.do_GET(self)
        else:
            query = parse_qs(parsed_path.query)
            command = query["command"][0]

            if command == "database":
                if not exists('database.sqlite'): # send no content if the database is missing
                    self.send_response(204)
                else:
                    self.success_response('text/plain', "database is present".encode())

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

# options and system configure
parser = argparse.ArgumentParser(description='Initiate a TREMPPI project.')
parser.add_argument('--path', help='specify the browsing location.')
parser.add_argument('--port', help='number of the port to run the browser on')
args = parser.parse_args()

EXEC_PATH, BIN_PATH, HOME_PATH, DEST_PATH = normal_paths(sys.argv[0], args)

# make sure data exist
generateFiles(join(DEST_PATH, data_folder))

# start the server and open the webpage
chdir(DEST_PATH)
if (args.port != None):
    port = args.port
else:
    port = default_port
replace_regex(join(DEST_PATH, "configuration.js"), "server_port: \\d*", "server_port: " + port)
server = HTTPServer(('', int(port)), StoreHandler)
webbrowser.open("http://localhost:" + port + "/index.html")
server.serve_forever()