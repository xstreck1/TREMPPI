__author__ = 'adams_000'

from os import replace, remove
from os.path import dirname, join
from urllib.parse import urlparse, parse_qs
from http.server import SimpleHTTPRequestHandler
from tremppi.header import configure_filename
from tremppi.file_manipulation import save_file, get_log, delete_project, get_path_level, read_jsonp, write_jsonp
from tool_manager import ToolManager
import subprocess

# TREMPPI server that communicates between HTML reports and the filesystem
class TremppiServer(SimpleHTTPRequestHandler):
    _tool_manager = ToolManager()

    def success_response(self, content_type, data):
        self.send_response(200)
        self.send_header('Content-type', content_type)
        self.end_headers()
        self.wfile.write(data)

    def error_response(self, content_type, data):
        self.send_response(400)
        self.send_header('Content-type', content_type)
        self.end_headers()
        self.wfile.write(data)

    # respond to the GET request
    def do_GET(self):
        parsed_url = urlparse(self.path)
        # print(parsed_path.path)
        data = ""
        if parsed_url.query == "" or parsed_url.query[0] == "_":
            if parsed_url.path == "/":
                data = "tremmpi browse is running"
            else:
                return SimpleHTTPRequestHandler.do_GET(self)
        elif parsed_url.query[0:len('getProgress')] == 'getProgress':
            data = self._tool_manager.get_progress()
        elif parsed_url.query[0:len('getLog')] == 'getLog':
            data = get_log("./log.txt")
        elif parsed_url.query[0:len("getCommands")] == "getCommands":
            data = self._tool_manager.get_commands()
        self.success_response('text', (str(data).encode()))

    # respond to the post request
    def do_POST(self):
        parsed_url = urlparse(self.path)
        parsed_path = parsed_url.path[1:] # remove the leading /
        if parsed_url.query == 'save':
            # writes the content of the message to the file specified by the URL
            length = self.headers['content-length']
            data = self.rfile.read(int(length))
            save_file(parsed_path, data)
            self.success_response('text', ("save success".encode()))
        elif parsed_url.query == 'killAll':
            self._tool_manager.kill_all(parsed_path)
            self.success_response('text', ("killAll success".encode()))
        elif parsed_url.query[0:len('tremppi+')] == 'tremppi+':
            self._tool_manager.add_to_queue(parsed_path, parsed_url.query[len('tremppi+'):])
            progress = self._tool_manager.get_progress()
            self.success_response('text', (str(progress).encode()))
        elif parsed_url.query[0:len('create')] == 'create':
            self._tool_manager.call_init(parsed_path)
            self.success_response('text', ("create success".encode()))
        elif parsed_url.query[0:len('delete')] == 'delete':
            if get_path_level(parsed_path) == 1:
                if self._tool_manager.is_free(parsed_url.path):
                    delete_project(parsed_path)
                    self.success_response('text', ("delete success".encode()))
                else:
                    self.error_response('text', ("the project " + parsed_path + " has running jobs, can't remove".encode()))
            else:
                remove(parsed_url)
                self.success_response('text', ("delete success".encode()))
        elif parsed_url.query[0:len('rename+')] == 'rename+':
            new_name = parsed_url.query[len('rename+'):]
            if get_path_level(parsed_path) == 1:
                if self._tool_manager.is_free(parsed_url.path):
                    header, configuration = read_jsonp(join(parsed_path, configure_filename))
                    configuration["project_name"] = new_name
                    write_jsonp(join(parsed_path, configure_filename), header, configuration)
                    replace(parsed_path, new_name)
                    self.success_response('text', ("rename success".encode()))
                else:
                    self.error_response('text', ("the project " + parsed_path + " has running jobs, can't rename".encode()))
            else:
                replace(parsed_path, join(dirname(parsed_path), new_name))
                self.success_response('text', ("rename success".encode()))