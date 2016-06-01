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
from threading import Thread
from urllib.parse import urlparse, parse_qs

from flask import Flask, render_template, render_template_string, request, send_from_directory, Config, redirect
from flask_mail import Mail
from flask_sqlalchemy import SQLAlchemy
from flask_user import login_required, UserManager, UserMixin, SQLAlchemyAdapter
from flask_user.forms import RegisterForm
from flask_wtf import RecaptchaField

from init.init import init
from browse.tool_manager import ToolManager
from browse.initiate_projects import mk_usr_proj
from browse.query_responses import do_get, do_post
from webconfig import ConfigClass
from tremppi.configure import configure
from tremppi.file_manipulation import copyanything, read_jsonp, write_jsonp
from tremppi.header import last_page_filename, data_folder, database_file, configure_filename, template_folder, system
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
    app = Flask(__name__, template_folder=join(system.BIN_PATH, template_folder))
    app.config.from_object(ConfigClass)

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
        target = join(system.DEST_PATH, current_user.username)
        if not isdir(target):
            makedirs(target)
        last_page = mk_usr_proj(target)
        return redirect(last_page)

    @app.route('/<path:path>', methods=['GET', 'POST'])
    @login_required
    def serve(path):
        if request.method=='GET':
            return do_get(request.path[1:])
        elif request.method=='POST':
            return do_post(request.path[1:])
        else:
            return 'unhandled request type'

    return app
