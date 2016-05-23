# Created by Adam Streck, 2013-2015, adam.streck@fu-berlin.de
#
# This file is part of the Toolkit for Reverse Engineering of Molecular Pathways
# via Parameter Identification (TREMPPI)
#
# This program is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with
# this program.  If not, see <http://www.gnu.org/licenses/>.

import webbrowser
import sys
import argparse
import subprocess
from os import chdir, listdir
from os.path import dirname, join, abspath, exists

if __name__ == "__main__":
    sys.path.append(dirname(dirname(abspath(sys.argv[0]))))
from tremppi.header import data_folder, default_port, configure_filename, projects_filename, last_page_filename, system_init, system
from tremppi.project_files import list_projects, write_projects, generate_data
from tremppi.file_manipulation import read_jsonp, write_jsonp
from server import create_app



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

# check for the last selection by the user
last_page = ""
if exists(last_page_filename):
    with open(last_page_filename, "r") as last_page_file:
        last_page = last_page_file.read()

project_path = ''
# the case the folder is empty
if not listdir('.'):
    process = subprocess.Popen([join(system.BIN_PATH, "tremppi")] + ['init'] + ['project_0'] + ['--path'] + [abspath('.')])
    process.wait()
    write_projects('.')
# the case that we are in a single project
if exists(configure_filename):
    generate_data(data_folder)
    # select the opening page
    if last_page is not "" and exists(last_page):
        project_path = last_page
    else:
        project_path = "index.html"
# the case we have multiple projects (or the folder was empty)
elif exists(projects_filename):
    # get all projects, create a new one if empty
    projects = list_projects(".")
    if not projects:
        raise Exception('Projects missing from the project folder. Empty the folder and restart TREMPPI.')

    # select the opening page
    if last_page is not "" and exists(last_page):
        project_path = last_page
    else:
        project_path = projects[0] + "/index.html"

    # generate data on all the projects
    for project_folder in projects:
        generate_data(join(project_folder, data_folder))
    write_projects('.')
else:
    raise Exception(configure_filename + " or " + projects_filename + " does not exist in " + system.DEST_PATH + ", which is not empty. Are you sure it's a correct path?")

if args.nopen is False:
    webbrowser.open("http://localhost:" + port + "/" + project_path)

# Execute the server itself.

# Start web server
app = create_app()
app.run(port=int(port), debug=False)
