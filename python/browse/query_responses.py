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

from flask import Flask, render_template, render_template_string, request, send_from_directory, Config, redirect,jsonify
from flask_mail import Mail
from flask_sqlalchemy import SQLAlchemy
from flask_user import login_required, UserManager, UserMixin, SQLAlchemyAdapter, current_user
from flask_user.forms import RegisterForm
from flask_wtf import RecaptchaField
from os import replace, remove, fdopen, makedirs
from os.path import dirname, join, basename, exists, abspath, split, isdir
from urllib.parse import urlparse, parse_qs

from init.init import init
from tool_manager import ToolManager
from server_errors import InvalidUsage, MethodNotAllowed, Conflict
from tremppi.configure import configure
from tremppi.file_manipulation import copyanything, read_jsonp, write_jsonp, path_is_parent
from tremppi.header import last_page_filename, data_folder, database_file, configure_filename, template_folder, system
from tremppi.project_files import write_projects, delete_project, save_file, get_log, get_path_level, is_project_folder

_tool_manager = ToolManager()

def do_get(app, url):
    path, file = split(url)
    if 'command' not in request.args:
        if request.path == "/":
            return "TREMPPI is running"
        else:
            return app.send_static_file(url)
            # if file.endswith('.html'):
            #     print('need html')
            #     conts = ''
            #     with open(path + '/' + file, "r") as foo:
            #         conts += foo.read()
            #     conts = conts.replace('<!DOCTYPE html>', '{% extends "base.html" %}<!DOCTYPE html>')
            #     conts = conts.replace('<title>', ' {% block header %}<title>')
            #     conts = conts.replace('<!--    </head>', '{% endblock %}<!--    </head>')
            #     conts = '{% extends "base.html" %}{% block header %}   <script src="./libs/jquery-2.1.3.js"></script>      <link rel="stylesheet" type="text/css" href="./libs/w2ui-1.4.3.css">        <script src="./libs/w2ui-1.4.3.js"></script>        <link rel="stylesheet" type="text/css" href="./libs/jquery.qtip.min.css">        <script src="./libs/jquery.qtip.min.js"></script>        <script src="./libs/paper-full.min.js"></script>        <script src="./common/tremppi.js"></script> <script src="./libs/cytoscape-2.3.9.js"></script>{% endblock %}' \
            #             '{% block content %}<div id="my_body"></div>{% endblock %}'
            #     print(conts)
            #     return render_template_string(conts)
            # else:
            #     return send_from_directory(path, file)  # VULNERABILITY, disable  ../ etc.
    elif request.args['command'] == 'getProgress':
        return str(_tool_manager.get_progress())
    elif request.args['command'] == 'getLog':
        return get_log(join(app.static_folder, path, "log.txt"))
    elif request.args['command'] == "getCommands":
        return _tool_manager.get_commands()
    else:
        raise InvalidUsage('unknown GET command ' + request.args['command'])


def do_post(app, url):
    path, file = split(url)
    if 'command' not in request.args:
        raise InvalidUsage('command missing from the POST request')
    elif request.args['command'] == 'init':
        if 'new_name' not in request.args:
            raise InvalidUsage('new_name missing from the init command')
        else:
            init(join(app.static_folder, request.args['new_name']))
            write_projects(app.static_folder)
    elif request.args['command'] == 'exit':
        def kill_me():
            print("shutdown")
        kill_me()
        #    server.shutdown()
        # Thread(target=kill_me, args=(self._server,)).start()
        return 'exit needs handling'
    elif request.args['command'] == 'save':
        # check if the path is safe to store to
        if not path_is_parent(app.static_folder, join(app.static_folder, url)):
            raise InvalidUsage('invalid save path ' + url)
        else:
            save_file(join(app.static_folder, url), request.json)
            return 'save successful'
    elif request.args['command'] == 'page':
        with open(join(app.static_folder, last_page_filename), 'w+') as last_page_file:
            last_page_file.seek(0)
            last_page_file.write(url)
            last_page_file.truncate()
            return 'last page write successful'
    elif request.args['command'] == 'killAll':
        _tool_manager.kill_all(url)
        return 'killAll successful'
    elif request.args['command'] == 'tremppi':
        if 'subcommand' not in request.args:
            raise InvalidUsage('subcommand not found in tremppi call')
        else:
            _tool_manager.add_to_queue(join(app.static_folder, path), request.args['subcommand'])
            progress = _tool_manager.get_progress()
            return str(progress)
    elif request.args['command'] == 'delete':
        if 'type' not in request.args:
            raise InvalidUsage('type missing in the delete command')
        elif request.args['type'] == 'folder':
            if not path_is_parent(app.static_folder, join(app.static_folder, path)):
                raise InvalidUsage('invalid delete path ' + join(app.static_folder, path))
            elif not is_project_folder(join(app.static_folder, path)):
                raise IvalidUsage(path + " seems not to be a TREMPPI project")
            elif _tool_manager.is_free(path):
                delete_project(join(app.static_folder, path))
                write_projects(app.static_folder)
                return 'delete successful'
            else:
                raise Conflict('jobs running on ' + names[0] + ', can not delete')
        elif request.args['type'] == 'file':
            remove(join(app.static_folder, path, file))
            configure(join(app.static_folder, dirname(path)), basename(path))
            return 'delete successful'
        else:
            raise InvalidUsage('unknown delete type: ' + request.args['type'])
    elif request.args['command'] == 'rename':
        if 'type' not in request.args:
            raise InvalidUsage('type missing in the rename command')
        elif request.args['type'] == 'folder':
            if 'new_name' not in request.args:
                raise InvalidUsage('new_name missing from the rename command')
            elif not path_is_parent(app.static_folder, join(app.static_folder, path)):
                raise InvalidUsage('invalid rename path ' + join(app.static_folder, path))
            elif not is_project_folder(join(app.static_folder, path)):
                raise IvalidUsage(path + " seems not to be a TREMPPI project")
            elif _tool_manager.is_free(path):
                file_path = join(app.static_folder, path, configure_filename)
                header, data = read_jsonp(file_path)
                replace(join(app.static_folder, path), join(app.static_folder, request.args['new_name']))
                data['project_name'] = request.args['new_name']
                file_path = join(app.static_folder, request.args['new_name'], configure_filename)
                write_jsonp(file_path, header, data)
                write_projects(app.static_folder)
                return 'rename successful'
            else:
                return 'jobs running on ' + path + ', can not rename'
        elif request.args['type'] == 'file':
            # TODO rename in the file
            replace(join(app.static_folder, path, file), join(app.static_folder, path, request.args['new_name'] + '.json'))
            configure(join(app.static_folder, dirname(path)), basename(path))
            return 'rename successful'
        else:
            raise InvalidUsage('unknown rename type: ' + request.args['type'])
    elif request.args['command'] == 'clone':
        if not path_is_parent(app.static_folder, join(app.static_folder, path)):
            raise InvalidUsage('invalid clone path ' + join(app.static_folder, path))
        elif not is_project_folder(join(app.static_folder, path)):
            raise InvalidUsage(path + " seems not to be a TREMPPI project")
        else:
            copyanything(join(app.static_folder, path), join(app.static_folder, path + '(clone)'))
            config_filepath = join(app.static_folder, path + '(clone)', configure_filename)
            header, data = read_jsonp(config_filepath)
            data['project_name'] = path + '(clone)'
            write_jsonp(config_filepath, header, data)
            write_projects(app.static_folder)
            return 'clone successful'
    elif request.args['command'] == 'finalize':
        if not path_is_parent(app.static_folder, join(app.static_folder, path)):
            raise InvalidUsage('invalid finalize path ' + path)
        elif not is_project_folder(join(app.static_folder, path)):
            raise IvalidUsage(path + " seems not to be a TREMPPI project")
        else:
            if exists(join(app.static_folder, path, data_folder, database_file)):
                remove(join(app.static_folder, path, data_folder, database_file))
            config_filepath = join(app.static_folder, path, configure_filename)
            header, data = read_jsonp(config_filepath)
            data['final'] = True
            write_jsonp(config_filepath, header, data)
            return 'finalize successful'
    else:
        raise InvalidUsage('unknown POST command ' + request.args['command'])




