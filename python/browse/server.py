__author__ = 'adams_000'

from os import curdir
from os.path import join, exists
from urllib.parse import urlparse, parse_qs
from http.server import SimpleHTTPRequestHandler
from file_manager import save_file

# TREMPPI server that communicates between HTML reports and the filesystem
class TremppiServer(SimpleHTTPRequestHandler):
    def success_response(self, content_type, data):
        self.send_response(200)
        self.send_header('Content-type', content_type)
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
        parsed_path = urlparse(self.path)
        length = self.headers['content-length']
        store_path = join(curdir, parsed_path.query)
        data = self.rfile.read(int(length))
        save_file(store_path, data)
        self.success_response('text', ("".encode()))