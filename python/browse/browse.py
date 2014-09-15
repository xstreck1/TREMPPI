import webbrowser
import pathlib
import re
import sys
import os
import argparse
from urllib.parse import urlparse
from os import curdir, listdir
from os.path import join as pjoin
from os.path import splitext
from http.server import BaseHTTPRequestHandler, HTTPServer
from shutil import copyfile

class StoreHandler(BaseHTTPRequestHandler):
    def get_files(self):
        files = listdir(".")
        files = [value for value in files if re.match("^((?!browse).)*html", value)]
        print(files)
        return "\n".join(files)

    def do_GET(self):
        parsed_path = urlparse(self.path)
        print("DEBUG")
        print(parsed_path.path)
        if parsed_path.query == "" and parsed_path.path == "/":
            self.send_response(200)
            self.send_header('Content-type', 'text/plain')
            self.end_headers()
            self.wfile.write(self.get_files().encode())
        elif parsed_path.path != "":
            store_path = "." + parsed_path.path
            fileName, fileExtension = splitext(store_path)
            fileExtension = fileExtension[1:]
            if (fileExtension == "js"):
                fileExtension = "javascript"
            with open(store_path) as fh:
                self.send_response(200)
                self.send_header('Content-type', 'text/' + fileExtension)
                self.end_headers()
                self.wfile.write(fh.read().encode())
        elif parsed_path.query != "":
            store_path = pjoin(curdir, parsed_path.query)
            with open(store_path) as fh:
                self.send_response(200)
                self.send_header('Content-type', 'text/js')
                self.end_headers()
                self.wfile.write(fh.read().encode())


def do_POST(self):
    length = self.headers['content-length']
    data = self.rfile.read(int(length))
    parsed_path = urlparse(self.path)
    store_path = pjoin(curdir, parsed_path.query)
    with open(store_path, 'w') as fh:
        fh.write(data.decode())

    self.send_response(200)

# define options
parser = argparse.ArgumentParser(description='Initiate a TREMPPI project.')
parser.add_argument('--dest', help='specify the location where the file gets created.')
args = parser.parse_args()

# find paths
sys.path.append(os.path.abspath(pjoin(sys.argv[0], "../..")))
from tremppi_common.file_manipulation import normal_paths, copyanything
EXEC_PATH, BIN_PATH, HOME_PATH = normal_paths(sys.argv[0])

if args.dest != None:
    DEST_PATH = os.path.normpath(args.dest)
else:
    DEST_PATH = os.getcwd()
SRC_PATH = pjoin(HOME_PATH, os.path.normpath("javascript/browse/"))

# copy data and rename content
if not os.path.exists(pjoin(DEST_PATH, "browse.html")):
    copyanything(pjoin(SRC_PATH, "browse"), pjoin(DEST_PATH, "browse"))
    copyfile(pjoin(SRC_PATH, "browse.html"), pjoin(DEST_PATH, "browse.html"))

webbrowser.open("http://localhost:8080/browse.html")
server = HTTPServer(('', 8080), StoreHandler)
server.serve_forever()