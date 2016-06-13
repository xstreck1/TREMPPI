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

import json
import shutil
from os import listdir, makedirs
from os.path import isdir, isfile
from os.path import join, exists
from .file_manipulation import copyanything
from .header import folders, widgets, source_folder, data_folder, system, version
from .file_manipulation import read_jsonp, write_jsonp
from .header import configure_filename, projects_filename
from .server_errors import InvalidUsage


# make sure all the data are present
def generate_data(data_path):
    if not exists(data_path):
        makedirs(data_path)
    for widget in widgets:
        # main json
        json_filename = join(data_path, widget + '.json')
        if not isfile(json_filename):
            with open(json_filename, 'w+') as json_file:
                if widget != 'select':
                    json.dump({}, json_file)
                else:
                    json.dump({
                        "records": [{
                            "name": "all",
                            "recid": 0,
                            "select": True
                        }]},
                        json_file)
        # config js
        js_filename = join(data_path, widget + '.js')
        if not isfile(js_filename):
            open(js_filename, 'w+').close()
        if not exists(join(data_path, widget)):
            makedirs(join(data_path, widget))


# return subdirectories that contain configure.js
def list_projects(projects_path):
    result = []
    subdirs = [name for name in listdir(projects_path) if isdir(join(projects_path, name))]
    for subdir in subdirs:
        if exists(join(projects_path, subdir, configure_filename)):
            result.append(subdir)
    return result


def write_projects(projects_path):
    if not isfile(join(projects_path, projects_filename)):
        with open(join(projects_path, projects_filename), 'w+') as file:
            file.write('tremppi.projects = [];')	
    header, data = read_jsonp(join(projects_path, projects_filename))
    data = list_projects(projects_path)
    write_jsonp(join(projects_path, projects_filename), header, data)


def is_project_folder(path):
    return isfile(join(path, configure_filename))


def delete_project(path):
    if not is_project_folder(path):
        raise InvalidUsage(path + " seems not to be a TREMPPI project")
    else:
        shutil.rmtree(path)


def save_file(store_path, data):
    with open(store_path, 'w') as file:
        json.dump(data, file)


def get_log_data(log_path):
    with open(log_path, 'r') as file:
        return file.read()


def get_path_level(path): #data files have level -1, project files 0, projects 1
    return 1 - path.count("/")


def tremppi_init(path, name):
    DEST_CONTENT = join(path, name)
    if exists(DEST_CONTENT):
        raise Exception('The destination folder ' + DEST_CONTENT + ' already exists, aborting.')

    # copy the data
    for folder in folders:
        source = join(join(system.HOME_PATH, source_folder), folder)
        destination = join(DEST_CONTENT, folder)
        copyanything(source, destination)

    for file in widgets:
        shutil.copy(join(system.HOME_PATH, source_folder, file + ".html"), DEST_CONTENT)

    shutil.copy(join(system.HOME_PATH, source_folder, "logo.png"), DEST_CONTENT)
    shutil.copy(join(system.HOME_PATH, source_folder, "favicon.ico"), DEST_CONTENT)

    # make the data directory
    generate_data(join(DEST_CONTENT, data_folder))

    # create the configure data
    with open(join(DEST_CONTENT, 'configure.js'), 'w+') as setup:
        setup.write('tremppi.setup = { ' +
                    '\t"project_name": "' + name + '",\n' +
                    '\t"version": "' + version + '"\n' +
                    '};'
                    )

    open(join(DEST_CONTENT, 'log.txt'), 'w+')