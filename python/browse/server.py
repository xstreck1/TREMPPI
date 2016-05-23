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





    # ROUTES
    @app.route('/')
    def home_page():
        return 'tremppi browse is running'

    @app.route('/<path:path>', methods=['GET'])
    def do_get(path):
        parsed_url = urlparse(path)
        print(parsed_url)
        return path

    @app.route('/<path:path>', methods=['POST'])
    def do_post(path):
        parsed_url = urlparse(path)
        print(parsed_url)
        return path




    return app
