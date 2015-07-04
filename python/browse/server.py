__author__ = 'adams_000'

from os import curdir
from os.path import join, exists, abspath
from urllib.parse import urlparse, parse_qs
from http.server import SimpleHTTPRequestHandler
from file_manager import save_file
from tool_manager import ToolManager

# TREMPPI server that communicates between HTML reports and the filesystem
class TremppiServer(SimpleHTTPRequestHandler):
    _tool_manager = ToolManager()

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
    def do_POST(self):
        parsed_url = urlparse(self.path)
        if parsed_url.query == "save":
            # writes the content of the message to the file specified by the URL
            length = self.headers['content-length']
            url_path = "." + parsed_url.path # The path starts with / so the dot must be prepended
            store_path = join(abspath(curdir), url_path)
            data = self.rfile.read(int(length))
            save_file(store_path, data)
            self.success_response('text', ("".encode()))
        if parsed_url.query[0:7] == "tremppi":
            print(parsed_url.query[8:])
            self._tool_manager.add_to_queue(parsed_url.query[8:])
            progress = self._tool_manager.get_progress()
            self.success_response('text', (str(progress).encode()))