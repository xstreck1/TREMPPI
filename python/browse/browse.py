import webbrowser
import sys
import argparse
import subprocess
from os import chdir, listdir
from os.path import dirname, join, abspath, exists
from http.server import HTTPServer

def set_port(path):
    file_path = join(path, configure_filename)
    header, data = read_jsonp(file_path)
    data['server_port'] = port
    write_jsonp(file_path, header, data)

if __name__ == "__main__":
    sys.path.append(dirname(dirname(abspath(sys.argv[0]))))
from tremppi.header import data_folder, default_port, configure_filename, projects_filename, system_init, system
from tremppi.project_files import list_projects, write_projects, generate_data
from server import TremppiServer
from tremppi.file_manipulation import read_jsonp, write_jsonp

# options and system configure
parser = argparse.ArgumentParser(description='Initiate a TREMPPI project.')
parser.add_argument('--path', help='specify the browsing location.')
parser.add_argument('--port', help='number of the port to run the browser on')
parser.add_argument('--nopen', help='if set, do not open the browser', action='store_true')

args = parser.parse_args()
system_init(sys.argv[0], args)

chdir(system.DEST_PATH)
if args.port is not None:
    port = args.port
else:
    port = default_port


project_path = ''
if exists(configure_filename):
    set_port('.')
    generate_data(join('.', data_folder))
elif exists(projects_filename) or listdir('.') == []:
    projects = list_projects(".")
    # create a new project if empty
    if len(projects) is 0:
        subprocess.Popen(join(system.BIN_PATH,"tremppi") + " init project_0", shell=True).wait()
        projects = ["project_0"]
    project_path = projects[0] + "/"
    # set ports on all the projects
    for project_folder in projects:
        set_port(project_folder)
        generate_data(join(project_folder, data_folder))
    write_projects('.')
else:
    raise Exception(configure_filename + " or " +  projects_filename + " does not exist in " + system.DEST_PATH + ". Are you sure it's a correct path?")

if args.nopen is False:
    webbrowser.open("http://localhost:" + port + "/" + project_path + "index.html")

# Execute the server itself.
server = HTTPServer(('', int(port)), TremppiServer)
server.serve_forever()