import webbrowser
import pathlib
import re
from urllib.parse import urlparse
from os import curdir, listdir
from os.path import join as pjoin
from os.path import splitext

from http.server import BaseHTTPRequestHandler, HTTPServer


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


p = pathlib.Path("browse.html")
if p.exists():
   webbrowser.open("http://localhost:8080/browse.html")

server = HTTPServer(('', 8080), StoreHandler)
server.serve_forever()