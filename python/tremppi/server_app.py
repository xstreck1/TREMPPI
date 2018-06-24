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

import pathlib
from os import makedirs
from os.path import join, exists, isdir, abspath
from flask import Flask, render_template, request, redirect, jsonify
from flask_mail import Mail
from flask_sqlalchemy import SQLAlchemy
from flask_user import login_required, UserManager, UserMixin, SQLAlchemyAdapter, current_user
from flask_login import logout_user
from flask_user.forms import RegisterForm
from flask_wtf import RecaptchaField
from wtforms import BooleanField, validators
from shutil import rmtree

from .initiate_projects import mk_usr_proj
from .server_errors import InvalidUsage, MethodNotAllowed, Conflict
from .header import template_folder, system, server_config_filename, model_size_limit, static_folder
from .query_responses import do_post, do_get
from .tool_manager import ToolManager


# Extend the basic registration with a recaptcha field in a custom class
class TremppiRegisterForm(RegisterForm):
    recaptcha = RecaptchaField()
    accept_tos = BooleanField('I agree to my e-mail address being stored on the application server. ' 
                              'I acknowledge that all data created within the application are going to be stored on the server.', [validators.DataRequired()])


# Setup Flask app and app.config
def create_app():
    print("App at: " + system.DEST_PATH)
    app = Flask(__name__, template_folder=join(system.BIN_PATH, template_folder), static_folder=join(system.BIN_PATH, static_folder))
    config_file = join(system.DEST_PATH, server_config_filename)
    if exists(config_file):
        app.config.from_pyfile(config_file)
    else:
        raise Exception('The configuration file ' + config_file + 'is missing')

    database_path = abspath(join(system.DEST_PATH, 'TREMPPI_users.sqlite'))
    app.config['SQLALCHEMY_DATABASE_URI'] = "sqlite:///" + database_path

    # Initialize Flask extensions
    db = SQLAlchemy(app)
    mail = Mail(app)

    class TremppiUser(db.Model, UserMixin):
        id = db.Column(db.Integer, primary_key=True)

        # User authentication information
        username = db.Column(db.String(50), nullable=False, unique=True)
        password = db.Column(db.String(255), nullable=False, server_default='')
        reset_password_token = db.Column(db.String(100), nullable=False, server_default='')

        # User email information
        email = db.Column(db.String(255), nullable=False, unique=True)
        email_confirmed_at = db.Column('confirmed_at', db.DateTime())

        # User information
        active = db.Column('is_active', db.Boolean(), nullable=False, server_default='0')
        first_name = db.Column(db.String(100), nullable=False, server_default='')
        last_name = db.Column(db.String(100), nullable=False, server_default='')

        # User limitations
        size_limit = db.Column('size_limit', db.Integer, nullable=False, server_default=str(model_size_limit))

    # Create all database tables
    db.create_all()

    # Setup Flask-User
    # Customize Flask-User
    class CustomUserManager(UserManager):
        def customize(self, app):
            # Override properties
            self.RegisterFormClass = TremppiRegisterForm

    user_manager = CustomUserManager(app, db, TremppiUser)

    def projects_path():
        if current_user.is_authenticated:
            return join(system.DEST_PATH, current_user.username)
        else:
            return MethodNotAllowed("Trying to access a project file without authentication.")
    app.projects_path = projects_path

    app.tools = {}


    def get_tool_manager():
        if current_user.is_authenticated:
            if current_user.username not in app.tools:
                app.tools[current_user.username] = ToolManager(False)
            return app.tools[current_user.username]
        else:
            return MethodNotAllowed("Trying to access a tool manager without authentication.")
    app.get_tool_manager = get_tool_manager

    # ROUTES
    @app.route('/')
    def home_page():
        if current_user is None or not current_user.is_authenticated:
            return render_template("index.html")
        else:
            return redirect('/members')

    @app.route('/members')
    @login_required
    def members_page():
        if not isdir(projects_path()):
            makedirs(projects_path())
        last_page = mk_usr_proj(projects_path())
        return redirect(last_page)

    @app.route('/user/delete')
    @login_required
    def delete_user():
        if current_user.is_authenticated:
            current_id = current_user.id
            print(current_id)
            project_path = projects_path()
            logout_user()
            TremppiUser.query.filter_by(id=current_id).delete()
            db.session.commit()
            rmtree(project_path)
            return redirect('/')

    @app.route('/user/profile')
    def redirect_to_new():
        return redirect('user/edit_user_profile')

    @app.route('/exec_type.js', methods=['GET'])
    def exec_type():
        return 'tremppi.exec_type = "server"'

    @app.route('/<path:path>', methods=['GET', 'POST'])
    @login_required
    def serve(path):
        if request.method == 'GET':
            return do_get(app, path)
        elif request.method == 'POST':
            return do_post(app, path)
        else:
            return 'unhandled request type'

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

    @app.errorhandler(Conflict)
    def handle_not_allowed(error):
        response = jsonify(error.to_dict())
        response.status_code = error.status_code
        return response

    #setup the rotating logger
    if not app.debug:
        import logging
        from logging.handlers import RotatingFileHandler
        file_handler = RotatingFileHandler(join(system.DEST_PATH, "TREMPPI.log"), maxBytes=65536, backupCount=9)
        file_handler.setLevel(logging.DEBUG)
        file_handler.setFormatter(logging.Formatter('[%(asctime)s] %(levelname)s in %(module)s: %(message)s'))
        app.logger.addHandler(file_handler)
        logging.getLogger('sqlalchemy').addHandler(file_handler)

    return app