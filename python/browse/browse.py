import webbrowser
import pathlib
import re
import sys
import os
import argparse
from urllib.parse import urlparse
from os import curdir, listdir
from os.path import splitext, dirname, join
from http.server import BaseHTTPRequestHandler, HTTPServer
from shutil import copyfile, rmtree


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
        else:
            mime_type = "text/" + file_extension
        with open(store_path) as fh:
            self.send_response(200)
            self.send_header('Content-type', mime_type)
            self.end_headers()
            self.wfile.write(fh.read().encode())

    # respond to the GET request
    def do_GET(self):
        parsed_path = urlparse(self.path)
        print(parsed_path.path)
        if parsed_path.query == "":
            if parsed_path.path == "/":
                self.send_response(200)
                self.send_header('Content-type', 'text/plain')
                self.end_headers()
                self.wfile.write("tremmpi browse is running".encode())
            else:
                store_path = "." + parsed_path.path
                self.provide_content(store_path)
        else:
            # get list of HTML files in the topmost directory
            if parsed_path.query == "files":
                self.send_response(200)
                self.send_header('Content-type', 'text/plain')
                self.end_headers()
                self.wfile.write(self.get_files().encode())
            # read content of a file under given url - used for loading models
            if parsed_path.query == "content":
                store_path = join(curdir, parsed_path.query)
                with open(store_path) as fh:
                    self.send_response(200)
                    self.send_header('Content-type', 'text/js')
                    self.end_headers()
                    self.wfile.write(fh.read().encode())
            # obtain columns of the database
            elif parsed_path.query == "columns":
                self.send_response(200)
                self.send_header('Content-type', 'text/plain')
                self.end_headers()
                self.wfile.write("obtain columns")


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

# define options
parser = argparse.ArgumentParser(description='Initiate a TREMPPI project.')
parser.add_argument('--dest', help='specify the browsing location.')
args = parser.parse_args()

# find paths
sys.path.append(dirname(dirname(os.path.abspath(sys.argv[0]))))
from tremppi_common.file_manipulation import copyanything, normal_paths

EXEC_PATH, BIN_PATH, HOME_PATH = normal_paths(sys.argv[0])

if args.dest != None:
    DEST_PATH = os.path.normpath(args.dest)
else:
    DEST_PATH = os.getcwd()
JAVASCRIPT_SOURCE = join(HOME_PATH, os.path.normpath("javascript"))

# refresh data if in editor
if listdir("..")[0] == ".idea":
    if os.path.exists(join(DEST_PATH, "browse.html")):
        rmtree(join(DEST_PATH, "browse"))
        os.remove(join(DEST_PATH, "browse.html"))
    copyanything(join(JAVASCRIPT_SOURCE, "browse"), join(DEST_PATH, "browse"))
    copyfile(join(JAVASCRIPT_SOURCE, "browse.html"), join(DEST_PATH, "browse.html"))

webbrowser.open("http://localhost:8080/browse.html")
server = HTTPServer(('', 8080), StoreHandler)
server.serve_forever()