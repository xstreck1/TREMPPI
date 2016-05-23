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


__author__ = 'Voydwalker'
import os, os.path, json
from flask import Flask, render_template, render_template_string, request, send_from_directory
from flask_mail import Mail
from flask_sqlalchemy import SQLAlchemy
from flask_user import login_required, UserManager, UserMixin, SQLAlchemyAdapter, current_user
from flask_user.forms import RegisterForm
from flask_wtf import RecaptchaField
from webconfig import ConfigClass
from urllib.parse import urlparse
import re
import sys
from os import replace, remove, fdopen
from os.path import dirname, join, basename, exists
from threading import Thread
from init.init import init
from tremppi.header import last_page_filename, data_folder, database_file, configure_filename
from tremppi.file_manipulation import copyanything, read_jsonp, write_jsonp
from tool_manager import ToolManager
from tremppi.configure import configure
from tremppi.project_files import write_projects, delete_project, save_file, get_log, get_path_level



def create_app():       # Setup Flask app and app.config
    app = Flask(__name__)
    app.config.from_object(ConfigClass)

    # Initialize Flask extensions
    db = SQLAlchemy(app)
    mail = Mail(app)

    # Define the User data model for SQLAlchemy
    class User(db.Model, UserMixin):
        id = db.Column(db.Integer, primary_key=True)

        # User authentication information
        username = db.Column(db.String(50), nullable=False, unique=True)
        password = db.Column(db.String(255), nullable=False, server_default='')
        reset_password_token = db.Column(db.String(100), nullable=False, server_default='')

        # User email information
        email = db.Column(db.String(255), nullable=False, unique=True)
        confirmed_at = db.Column(db.DateTime())

        # User information
        active = db.Column('is_active', db.Boolean(), nullable=False, server_default='0')
        first_name = db.Column(db.String(100), nullable=False, server_default='')
        last_name = db.Column(db.String(100), nullable=False, server_default='')

    # Extend the basic registration with a recaptcha field in a custom class
    class MyRegisterForm(RegisterForm):
        recaptcha = RecaptchaField()

    # Create all database tables
    db.create_all()

    # Setup Flask-User
    db_adapter = SQLAlchemyAdapter(db, User)
    user_manager = UserManager(db_adapter, app, register_form=MyRegisterForm)

    _tool_manager = ToolManager()





    # ROUTES
    @app.route('/')
    def home_page():
        return 'tremppi browse is running'

    @app.route('/<path:path>', methods=['GET'])
    #@login_required
    def do_get(path):
        parsed_url = urlparse(path)
        print(parsed_url.path)
        pt,fl=os.path.split(parsed_url.path)
        print(pt)
        print(fl)
        data = ""
        if parsed_url.query == "" or parsed_url.query[0] == "_":
            if parsed_url.path == "/":
                data = "tremmpi browse is running"
            else:   #THIS PART SEEMS TO MALFUNCTION
                return send_from_directory(pt, fl)  #VULNERABILITY, disable  ../ etc.
                #return '_ needs handling'
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
        return data     #str(data).encode()









    @app.route('/<path:path>', methods=['POST'])
    #@login_required
    def do_post(path):
        parsed_url = urlparse(path)
        print(parsed_url)
        return path




    return app
