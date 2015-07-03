import webbrowser
import sys
import argparse
from os import chdir
from os.path import dirname, join, abspath
from http.server import HTTPServer

if __name__ == "__main__":
    sys.path.append(dirname(dirname(abspath(sys.argv[0]))))
from tremppi.file_manipulation import replace_regex, normal_paths
from tremppi.header import data_folder, default_port
from init.init import generate_data
from server import TremppiServer

# options and system configure
parser = argparse.ArgumentParser(description='Initiate a TREMPPI project.')
parser.add_argument('--path', help='specify the browsing location.')
parser.add_argument('--port', help='number of the port to run the browser on')
args = parser.parse_args()

EXEC_PATH, BIN_PATH, HOME_PATH, DEST_PATH = normal_paths(sys.argv[0], args)

# make sure data exist
generate_data(join(DEST_PATH, data_folder))

# start the server and open the HTML page.
chdir(DEST_PATH)
if args.port is not None:
    port = args.port
else:
    port = default_port
replace_regex(join(DEST_PATH, "configure.js"), "server_port: \\d*", "server_port: " + port)
server = HTTPServer(('', int(port)), TremppiServer)
webbrowser.open("http://localhost:" + port + "/index.html")

# Execute the server itself.
server.serve_forever()