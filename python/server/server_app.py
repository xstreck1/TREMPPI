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

from flask import Flask, render_template, render_template_string, request, send_from_directory, Config, redirect, jsonify
from flask_mail import Mail
from flask_sqlalchemy import SQLAlchemy
from flask_user import login_required, UserManager, UserMixin, SQLAlchemyAdapter, current_user
from flask_user.forms import RegisterForm
from flask_wtf import RecaptchaField
from os import replace, remove, fdopen, makedirs
from os.path import dirname, join, basename, exists, abspath, split, isdir
from threading import Thread
from urllib.parse import urlparse, parse_qs

from browse.initiate_projects import mk_usr_proj
from browse.query_responses import do_get, do_post, InvalidUsage, MethodNotAllowed
from browse.tool_manager import ToolManager
from init.init import init
from tremppi.configure import configure
from tremppi.file_manipulation import copyanything, read_jsonp, write_jsonp
from tremppi.header import last_page_filename, data_folder, database_file, configure_filename, template_folder, system, server_config_filename
from tremppi.project_files import write_projects, delete_project, save_file, get_log, get_path_level


# Define the User data model for SQLAlchemy
def make_tremppi_user(db):
    class TremppiUser(db.Model, UserMixin):
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
    return TremppiUser


# Extend the basic registration with a recaptcha field in a custom class
class TremppiRegisterForm(RegisterForm):
    recaptcha = RecaptchaField()


# Setup Flask app and app.config
def create_app():
    print(system.DEST_PATH)
    app = Flask(__name__, template_folder=join(system.BIN_PATH, template_folder), static_folder=system.DEST_PATH)
    config_file = join(system.DEST_PATH, server_config_filename)
    if exists(config_file):
        app.config.from_pyfile(config_file)
    else:
        raise Exception('The configuration file ' + config_file + 'is missing')

    # Initialize Flask extensions
    db = SQLAlchemy(app)
    mail = Mail(app)

    # Create all database tables
    db.create_all()

    # Setup Flask-User
    db_adapter = SQLAlchemyAdapter(db, make_tremppi_user(db))
    user_manager = UserManager(db_adapter, app, register_form=TremppiRegisterForm)

    # ROUTES

    @app.route('/')
    def home_page():
        return render_template('index.html')

    @app.route('/members')
    @login_required
    def members_page():
        app.static_folder = join(system.DEST_PATH, current_user.username)
        if not isdir(system.DEST_PATH):
            makedirs(system.DEST_PATH)
        last_page = mk_usr_proj(system.DEST_PATH)
        return redirect(last_page)

    @app.route('/<path:path>', methods=['GET', 'POST'])
    @login_required
    def serve(path):
        # path = current_user.username + '/' + path
        if request.method == 'GET':
            return do_get(app, path)
        elif request.method == 'POST':
            return do_post(app, path)
        else:
            raise MethodNotAllowed()

    @app.before_first_request
    def _run_on_start():
        if current_user.is_authenticated :
            app.static_folder = join(system.DEST_PATH, current_user.username)

    @app.errorhandler(InvalidUsage)
    def handle_invalid_usage(error):
        response = jsonify(error.to_dict())
        response.status_code = error.status_code
        return response

    @app.errorhandler(MethodNotAllowed)
    def handle_not_allowed(error):
        response = jsonify(error.to_dict())
        response.status_code = error.status_code
        return response

    return app
