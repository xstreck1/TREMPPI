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

import sys
import zipfile
import shutil
import sqlite3
import json
from os import replace, remove, makedirs, pathsep
from os.path import dirname, join, basename, exists, split, commonprefix

from flask import request, send_from_directory
from werkzeug.utils import secure_filename

from .project_files import tremppi_init
from .configure import configure
from .file_manipulation import copyanything, read_jsonp, write_jsonp, path_is_parent, zipdir
from .header import last_page_filename, data_folder, database_file, configure_filename, system
from .project_files import write_projects, delete_project, save_file, get_log_data, is_project_folder
from .server_errors import InvalidUsage, Conflict
from .db2sbml import writeDBModelToSBML, database_to_editor
from .sbml2db import writeSBMLToDBModel
from .database_reader import read_regulations, read_components

def wrong_get(app, url):
    raise InvalidUsage('unknown GET command ' + request.args['command'])


def get_progress(app,  url):
    return str(app.get_tool_manager().get_progress())


def get_log(app, url):
    path, file = split(url)
    return get_log_data(join(app.projects_path(), path, "log.txt"))


def get_commands(app, url):
    return app.get_tool_manager().get_commands()


def wrong_post(app, url):
    raise InvalidUsage('unknown POST command ' + request.args['command'])


def new_project(app, url):
    if 'new_name' not in request.args:
        raise InvalidUsage('new_name missing from the init command')
    else:
        tremppi_init(app.projects_path(), request.args['new_name'])
        write_projects(app.projects_path())
        return 'project ' + request.args['new_name'] + ' created successfully'


def exit(app,  url):
    func = request.environ.get('werkzeug.server.shutdown')
    if func is None:
        raise RuntimeError('Not running with the Werkzeug Server')
    func()
    return 'exit needs handling'


def save(app,  url):
    # check if the path is safe to store to
    if not path_is_parent(app.projects_path(), join(app.projects_path(), url)):
        raise InvalidUsage('invalid save path ' + url)
    else:
        save_file(join(app.projects_path(), url), request.json)
        return 'save successful'


def page(app, url):
    with open(join(app.projects_path(), last_page_filename), 'w+') as last_page_file:
        last_page_file.seek(0)
        last_page_file.write(url)
        last_page_file.truncate()
        return 'last page write successful'


def kill_all(app, url):
        app.get_tool_manager().kill_all(url)
        return 'kill_all successful'


def tremppi(app, url):
    path, file = split(url)
    if 'subcommand' not in request.args:
        raise InvalidUsage('subcommand not found in tremppi call')
    else:
        app.get_tool_manager().add_to_queue(join(app.projects_path(), path), request.args['subcommand'])
        progress = app.get_tool_manager().get_progress()
        return str(progress)


def delete(app, url):
    path, file = split(url)
    if 'type' not in request.args:
        raise InvalidUsage('type missing in the delete command')
    elif request.args['type'] == 'folder':
        if not path_is_parent(app.projects_path(), join(app.projects_path(), path)):
            raise InvalidUsage('invalid delete path ' + join(app.projects_path(), path))
        elif not is_project_folder(join(app.projects_path(), path)):
            raise InvalidUsage(path + " seems not to be a TREMPPI project")
        elif app.get_tool_manager().is_free(path):
            delete_project(join(app.projects_path(), path))
            write_projects(app.projects_path())
            return 'delete successful'
        else:
            raise Conflict('jobs running on ' + path + ', can not delete')
    elif request.args['type'] == 'file':
        remove(join(app.projects_path(), path, file))
        configure(join(app.projects_path(), dirname(path)), basename(path))
        return 'delete successful'
    else:
        raise InvalidUsage('unknown delete type: ' + request.args['type'])


def rename(app, url):
    path, file = split(url)
    if 'type' not in request.args:
        raise InvalidUsage('type missing in the rename command')
    elif request.args['type'] == 'folder':
        if 'new_name' not in request.args:
            raise InvalidUsage('new_name missing from the rename command')
        elif not path_is_parent(app.projects_path(), join(app.projects_path(), path)):
            raise InvalidUsage('invalid rename path ' + join(app.projects_path(), path))
        elif not is_project_folder(join(app.projects_path(), path)):
            raise InvalidUsage(path + " seems not to be a TREMPPI project")
        elif app.get_tool_manager().is_free(path):
            file_path = join(app.projects_path(), path, configure_filename)
            header, data = read_jsonp(file_path)
            replace(join(app.projects_path(), path), join(app.projects_path(), request.args['new_name']))
            data['project_name'] = request.args['new_name']
            file_path = join(app.projects_path(), request.args['new_name'], configure_filename)
            write_jsonp(file_path, header, data)
            write_projects(app.projects_path())
            return 'rename successful'
        else:
            return 'jobs running on ' + path + ', can not rename'
    elif request.args['type'] == 'file':
        # TODO rename in the file
        replace(join(app.projects_path(), path, file), join(app.projects_path(), path, request.args['new_name'] + '.json'))
        configure(join(app.projects_path(), dirname(path)), basename(path))
        return 'rename successful'
    else:
        raise InvalidUsage('unknown rename type: ' + request.args['type'])


def clone(app, url):
    path, file = split(url)
    if not path_is_parent(app.projects_path(), join(app.projects_path(), path)):
        raise InvalidUsage('invalid clone path ' + join(app.projects_path(), path))
    elif not is_project_folder(join(app.projects_path(), path)):
        raise InvalidUsage(path + " seems not to be a TREMPPI project")
    else:
        copyanything(join(app.projects_path(), path), join(app.projects_path(), path + '(clone)'))
        config_filepath = join(app.projects_path(), path + '(clone)', configure_filename)
        header, data = read_jsonp(config_filepath)
        data['project_name'] = path + '(clone)'
        write_jsonp(config_filepath, header, data)
        write_projects(app.projects_path())
        return 'clone successful'


def finalize(app, url):
    path, file = split(url)
    if not path_is_parent(app.projects_path(), join(app.projects_path(), path)):
        raise InvalidUsage('invalid finalize path ' + path)
    elif not is_project_folder(join(app.projects_path(), path)):
        raise InvalidUsage(path + " seems not to be a TREMPPI project")
    else:
        if exists(join(app.projects_path(), path, data_folder, database_file)):
            remove(join(app.projects_path(), path, data_folder, database_file))
        config_filepath = join(app.projects_path(), path, configure_filename)
        header, data = read_jsonp(config_filepath)
        data['final'] = True
        write_jsonp(config_filepath, header, data)
        return 'finalize successful'


def download(app, url):
    path, file = split(url)
    if not path_is_parent(app.projects_path(), join(app.projects_path(), path)):
        raise InvalidUsage('invalid download path ' + path)
    elif not is_project_folder(join(app.projects_path(), path)):
        raise InvalidUsage(path + " seems not to be a TREMPPI project")
    else:
        zipf = zipfile.ZipFile(join(app.projects_path(), path) + '.zip', 'w', zipfile.ZIP_DEFLATED)
        zipdir(join(app.projects_path(), path), zipf)
        zipf.close()

        return 'zipping successful'


def upload(app, url):
    if 'file' not in request.files:
        raise InvalidUsage('No file attached')
    file = request.files['file']
    if secure_filename(file.filename)[-4:] !='.zip':
        raise InvalidUsage('The uploaded file is not a zipfile.')
    target_folder = join(app.projects_path(), secure_filename(file.filename)[:-4])
    if not commonprefix([target_folder, app.projects_path()]) == app.projects_path(): # out of the server folder
        raise InvalidUsage('Invalid filename')
    if not exists(target_folder):
        makedirs(target_folder)
    else:
        raise InvalidUsage('Project of the same name already exists.')
    try:
        zip_ref = zipfile.ZipFile(file, 'r')
    except:
        raise InvalidUsage('The file ' + file + ' is not recognised as a zip file.')
    try:
        zip_ref.extractall(target_folder)
    except:
        raise InvalidUsage('The extraction of the file ' + file + ' failed.')
    if not exists(join(target_folder, configure_filename)):
        shutil.rmtree(target_folder)
        raise InvalidUsage('The file does not contain a tremppi project.')
    write_projects(app.projects_path())
    return 'upload successful'


def importSBML(app, url):
    if 'file' not in request.files:
        raise InvalidUsage('No file attached')
    file = request.files['file']
    if secure_filename(file.filename)[-5:]  != '.sbml':
        raise InvalidUsage('The uploaded file is not an sbml file')
    project_name = secure_filename(file.filename)[:-5]
    target_folder = join(app.projects_path(), project_name)
    if not commonprefix([target_folder, app.projects_path()]) == app.projects_path():
        raise InvalidUsage('Invalid filename')
    if not exists(target_folder):
        tremppi_init(app.projects_path(), project_name)
    else:
        raise InvalidUsage('Project of the same name already exists.')

    try:
        sbml_file = join(target_folder, secure_filename(file.filename))
        file.save(sbml_file)
    except:
        shutil.rmtree(target_folder)
        raise InvalidUsage('Failed to save the file ' + file.filename)

    try:
        writeSBMLToDBModel(join(target_folder, data_folder, database_file), sbml_file)
        with sqlite3.connect(join(target_folder, data_folder, database_file)) as conn:
            comps = read_components(conn)
            regs = read_regulations(conn)
            json_root = database_to_editor(comps, regs)
            json_string = json.dumps(json_root)
        with open(join(target_folder, data_folder, 'editor.json'), "w+") as editor_file:
            editor_file.write(json_string)
    except:
        shutil.rmtree(target_folder)
        raise InvalidUsage('Failed to convert the file ' + file.filename + ' error: ' + sys.exc_info()[0])

    write_projects(app.projects_path())
    return 'import successful'


def exportSBML(app, url):
    path, file = split(url)
    if not path_is_parent(app.projects_path(), join(app.projects_path(), path)):
        raise InvalidUsage('invalid download path ' + path)
    elif not is_project_folder(join(app.projects_path(), path)):
        raise InvalidUsage(path + " seems not to be a TREMPPI project")
    else:
        writeDBModelToSBML(join(app.projects_path(), path, data_folder, database_file), join(app.projects_path(), path) + '.sbml')

        return 'export successful'


def do_post(app, url):
    if 'command' not in request.args:
        raise InvalidUsage('command missing from the POST request')
    else:
        return getattr(sys.modules[__name__], request.args['command'], wrong_get)(app, url)


def do_get(app, url):
    path, file = split(url)
    return send_from_directory(join(app.projects_path(), path), file)