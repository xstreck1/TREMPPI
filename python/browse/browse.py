import webbrowser
import sys
import argparse
import json
from os import chdir
from os.path import dirname, join, abspath, exists
from http.server import HTTPServer

if __name__ == "__main__":
    sys.path.append(dirname(dirname(abspath(sys.argv[0]))))
from tremppi.file_manipulation import replace_regex, normal_paths
from tremppi.header import data_folder, default_port, configure_filename
from init.init import generate_data, list_projects
from server import TremppiServer
from tremppi.file_manipulation import read_jsonp, write_jsonp

# options and system configure
parser = argparse.ArgumentParser(description='Initiate a TREMPPI project.')
parser.add_argument('--path', help='specify the browsing location.')
parser.add_argument('--port', help='number of the port to run the browser on')
parser.add_argument('--nopen', help='if set, do not open the browser', action='store_true')

args = parser.parse_args()

EXEC_PATH, BIN_PATH, HOME_PATH, DEST_PATH = normal_paths(sys.argv[0], args)

chdir(DEST_PATH)
if args.port is not None:
    port = args.port
else:
    port = default_port

# check for the configure file
if not exists(configure_filename):
    raise configure_filename + "  does not exist in " + configure_filename + ". Are you sure it's a correct path?"
else:
    project_path = ""
    header, configuration = read_jsonp(configure_filename)
    if "level" not in configuration:
        raise "Level not specified in the configure.js file."
    elif int(configuration['level']) == 1:
        configuration['projects_name'] = list_projects(".")
        project_path = configuration['projects_name'][0] + "/"
    configuration['server_port'] = port
    write_jsonp(configure_filename, header, configuration)

    server = HTTPServer(('', int(port)), TremppiServer)

    if args.nopen is False:
        webbrowser.open("http://localhost:" + port + "/" + project_path + "index.html")

    # Execute the server itself.
    server.serve_forever()