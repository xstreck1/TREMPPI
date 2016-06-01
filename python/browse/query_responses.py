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

from os import replace, remove, fdopen, makedirs
from os.path import dirname, join, basename, exists, abspath, split, isdir
from urllib.parse import urlparse, parse_qs


from flask import Flask, render_template, render_template_string, request, send_from_directory, Config, redirect
from flask_mail import Mail
from flask_sqlalchemy import SQLAlchemy
from flask_user import login_required, UserManager, UserMixin, SQLAlchemyAdapter, current_user
from flask_user.forms import RegisterForm
from flask_wtf import RecaptchaField

from init.init import init
from tool_manager import ToolManager
from tremppi.configure import configure
from tremppi.file_manipulation import copyanything, read_jsonp, write_jsonp
from tremppi.header import last_page_filename, data_folder, database_file, configure_filename, template_folder, system
from tremppi.project_files import write_projects, delete_project, save_file, get_log, get_path_level

_tool_manager = ToolManager()

def do_get(path):
    print(request)
    print(request.args)
    parsed_url = urlparse(path)
    pt, fl = split(parsed_url.path)
    pt = join(abspath(system.DEST_PATH), current_user.username, pt)
    print(pt)
    print(fl)
    data = ""
    if not request.args or '_' in request.args:  # works
        if request.path == "/":
            data = "tremmpi browse is running"
        else:
            if fl.endswith('.html'):
                print('need html')
                conts = ''
                with open(pt + '/' + fl, "r") as foo:
                    conts += foo.read()
                conts = conts.replace('<!DOCTYPE html>', '{% extends "base.html" %}<!DOCTYPE html>')
                conts = conts.replace('<title>', ' {% block header %}<title>')
                conts = conts.replace('<!--    </head>', '{% endblock %}<!--    </head>')
                conts = '{% extends "base.html" %}{% block header %}   <script src="./libs/jquery-2.1.3.js"></script>      <link rel="stylesheet" type="text/css" href="./libs/w2ui-1.4.3.css">        <script src="./libs/w2ui-1.4.3.js"></script>        <link rel="stylesheet" type="text/css" href="./libs/jquery.qtip.min.css">        <script src="./libs/jquery.qtip.min.js"></script>        <script src="./libs/paper-full.min.js"></script>        <script src="./common/tremppi.js"></script> <script src="./libs/cytoscape-2.3.9.js"></script>{% endblock %}' \
                        '{% block content %}<div id="my_body"></div>{% endblock %}'
                print(conts)
                return render_template_string(conts)
            else:
                return send_from_directory(pt, fl)  # VULNERABILITY, disable  ../ etc.
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


def do_post(req):
    print(request)
    print(request.args)  # holds the query key-values
    print(request.json)  # holds the data
    parsed_url = ''
    parsed_path = ''
    if request.args.get('exit'):
        def kill_me():
            print("shutdown")

        kill_me()
        #    server.shutdown()
        # Thread(target=kill_me, args=(self._server,)).start()
        return 'exit needs handling'
    elif 'save' in request.args:  # works
        save_file(req, request.json)
        return 'save needs handling'
    elif 'page' in request.args:  # works
        with open(last_page_filename, 'w+') as last_page_file:
            last_page_file.seek(0)
            last_page_file.write(req)
            last_page_file.truncate()
            return 'Error getting page'
    elif parsed_url.query == 'killAll':
        _tool_manager.kill_all(parsed_path)
        return 'killall needs handling'
    elif parsed_url.query[0:len('tremppi+')] == 'tremppi+':
        _tool_manager.add_to_queue(parsed_path, parsed_url.query[len('tremppi+'):])
        progress = _tool_manager.get_progress()
        return 'trmppi needs handling'
    elif parsed_url.query[0:len('delete')] == 'delete':
        remove(parsed_path)
        configure(dirname(dirname(parsed_path)), basename(dirname(parsed_path)))
        return 'delete needs handling'
    elif parsed_url.query[0:len('rename+')] == 'rename+':
        new_name = parsed_url.query[len('rename+'):]
        replace(parsed_path, join(dirname(parsed_path), new_name + '.json'))
        configure(dirname(dirname(parsed_path)), basename(dirname(parsed_path)))
        return 'rename needs handling'
