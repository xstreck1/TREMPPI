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

from os.path import dirname, abspath, join, normpath, basename, isfile
from os import getcwd

version = "1.1.0"

model_size_limit = 1000000

default_port = "8080"
configure_filename = "configure.js"
projects_filename = "projects.js"
last_page_filename = "last_page.txt"
server_config_filename = 'tremppi_server.ini'
database_file = 'database.sqlite'
properties_table = "properties"
source_folder = 'javascript'
template_folder = 'templates'
static_folder = 'static'
server_folder = 'server'
data_folder = 'data'
widgets = ['index', 'properties', 'editor', 'select', 'witness', 'regulations', 'correlations', 'quantitative', 'qualitative', 'group', 'tools']
folders = ['common', 'libs', 'index', 'properties', 'editor', 'select', 'witness', 'regulations', 'correlations',  'quantitative', 'qualitative', 'group', 'tools']
model_file = 'editor.json'


class System:
    EXEC_PATH = ''
    BIN_PATH = ''
    HOME_PATH = ''
    DEST_PATH = ''
system = System()


def system_init(exec_path, args):
    global system
    system.EXEC_PATH = abspath(exec_path)
    if basename(exec_path) == 'tremppi' or basename(exec_path) == 'tremppi.exe':
        system.BIN_PATH = abspath(dirname(exec_path))
        system.HOME_PATH = system.BIN_PATH
    elif isfile(join(dirname(dirname(exec_path)), 'tremppi')) or isfile(join(dirname(dirname(exec_path)), 'tremppi.exe')):
        system.BIN_PATH = abspath(dirname(dirname(exec_path)))
        system.HOME_PATH = system.BIN_PATH
    elif isfile(join(dirname(dirname(exec_path)), join('Release','tremppi.exe'))):
        system.BIN_PATH = abspath(join(dirname(dirname(exec_path)), 'Release'))
        system.HOME_PATH = dirname(system.BIN_PATH)
    else:
        raise Exception('TREMPPI binary not found')
    if args.path is not None:
        system.DEST_PATH = normpath(args.path)
    else:
        system.DEST_PATH = getcwd()
    return system
