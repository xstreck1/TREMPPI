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

from flask import Flask, render_template, render_template_string, request, send_from_directory, Config, redirect
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
from tremppi.configure import configure
from tremppi.file_manipulation import copyanything, read_jsonp, write_jsonp
from tremppi.header import last_page_filename, data_folder, database_file, configure_filename, template_folder, system
from tremppi.project_files import write_projects, delete_project, save_file, get_log, get_path_level

_tool_manager = ToolManager()

from flask import jsonify


class InvalidUsage(Exception):
    status_code = 400

    def __init__(self, message, status_code=None, payload=None):
        Exception.__init__(self)
        self.message = message
        if status_code is not None:
            self.status_code = status_code
        self.payload = payload

    def to_dict(self):
        rv = dict(self.payload or ())
        rv['message'] = self.message
        return rv

class MethodNotAllowed(Exception):
        status_code = 405

        def __init__(self, message, status_code=None, payload=None):
            Exception.__init__(self)
            self.message = message
            if status_code is not None:
                self.status_code = status_code
            self.payload = payload

        def to_dict(self):
            rv = dict(self.payload or ())
            rv['message'] = self.message
            return rv

def do_get(app, url):
    parsed_url = urlparse(url)
    path, file = split(parsed_url.path)
    data = ""
    if not request.args or '_' in request.args:  # works
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
    elif parsed_url.query[0:len("delete+")] == "delete+":
        names = parsed_url.query.split("+")
        if _tool_manager.is_free(names[1]):
            delete_project(names[1])
            write_projects('.')
            return 'delete+ needs handling'
        else:
            return 'jobs running on ' + names[0] + ', can not delete'
    elif parsed_url.query[0:len("init+")] == "init+":
        names = parsed_url.query.split("+")
        init(names[1])
        write_projects('.')
        return 'init+ needs handling'
    elif parsed_url.query[0:len("rename+")] == "rename+":
        names = parsed_url.query.split("+")
        if _tool_manager.is_free(names[1]):
            file_path = join(names[1], configure_filename)
            header, data = read_jsonp(file_path)
            data['project_name'] = names[2]
            write_jsonp(file_path, header, data)
            replace(names[1], names[2])
            write_projects('.')
            return 'rename+ needs handling'
        else:
            return 'jobs running on ' + names[0] + ', can not rename'
    elif parsed_url.query[0:len("clone+")] == "clone+":
        names = parsed_url.query.split("+")
        file_path = join(names[1], configure_filename)
        header, data = read_jsonp(file_path)
        data['project_name'] = names[1] + '(clone)'
        write_jsonp(file_path, header, data)
        copyanything(names[1], names[1] + '(clone)')
        file_path = join(names[1], configure_filename)
        header, data = read_jsonp(file_path)
        data['project_name'] = names[1]
        write_jsonp(file_path, header, data)
        write_projects('.')
        return 'clone+ needs handling'
    elif parsed_url.query[0:len("finalize+")] == "finalize+":
        names = parsed_url.query.split("+")
        if exists(join(names[1], data_folder, database_file)):
            remove(join(names[1], data_folder, database_file))
        file_path = join(names[1], configure_filename)
        header, data = read_jsonp(file_path)
        data['final'] = True
        write_jsonp(file_path, header, data)
        return 'finalize+ needs handling'
    elif parsed_url.query[0:len('getProgress')] == 'getProgress':
        data = _tool_manager.get_progress()
    elif parsed_url.query[0:len('getLog')] == 'getLog':
        data = get_log("./" + parsed_url.path[1:] + "log.txt")
    elif parsed_url.query[0:len("getCommands")] == "getCommands":
        data = _tool_manager.get_commands()
    return data  # str(data).encode()


def do_post(app, url):
    raise InvalidUsage('test')
    print(request.args)
    if request.args['command'] == 'exit':
        def kill_me():
            print("shutdown")
        kill_me()
        #    server.shutdown()
        # Thread(target=kill_me, args=(self._server,)).start()
        return 'exit needs handling'
    elif request.args['command'] == 'save':
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
    elif request.args['command'] == 'tremmpi':
        _tool_manager.add_to_queue(url, request['args'])
        progress = _tool_manager.get_progress()
        return 'TREMPPI ' + request['args'] + ' successful'
    elif request.args['command'] == 'delete':
        if 'type' not in request.args:
            raise InvalidUsage('type missing in the delete command')
        elif request.args['type'] == 'folder':
            raise InvalidUsage('notimplemented')
        elif request.args['type'] == 'file':
            remove(parsed_path)
            configure(dirname(dirname(url)), basename(dirname(url)))
            return 'delete successful'
        else:
            raise InvalidUsage('unknown delete type: ' + request.args['type'])
    elif request.args['command'] == 'rename':
        if 'type' not in request.args:
            raise InvalidUsage('type missing in the rename command')
        elif request.args['type'] == 'folder':
            raise InvalidUsage('notimplemented')
        elif request.args['type'] == 'file':
            replace(parsed_path, join(dirname(url), request.args['new_name'] + '.json'))
            configure(dirname(dirname(url)), basename(dirname(url)))
            return 'rename successful'
        else:
            raise InvalidUsage('unknown rename type: ' + request.args['type'])


