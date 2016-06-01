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


from argparse import ArgumentParser
from flask import Flask, render_template, render_template_string, request, send_from_directory, redirect, Config
from flask_mail import Mail
from flask_sqlalchemy import SQLAlchemy
from flask_user import login_required, UserManager, UserMixin, SQLAlchemyAdapter, current_user
from flask_user.forms import RegisterForm
from flask_wtf import RecaptchaField
from os import replace, remove, fdopen, chdir, listdir
from os.path import dirname, join, basename, exists, abspath, split
from sys import path, argv
from threading import Thread
from urllib.parse import urlparse, parse_qs

if __name__ == "__main__":
    path.append(dirname(dirname(abspath(argv[0]))))

from browse.web import create_app
from webconfig import ConfigClass
from browse.tool_manager import ToolManager
from init.init import init
from tremppi.header import data_folder, default_port, configure_filename, projects_filename, last_page_filename, system_init, system,  database_file
from tremppi.project_files import list_projects, write_projects, generate_data,  delete_project, save_file, get_log, get_path_level
from tremppi.file_manipulation import copyanything, read_jsonp, write_jsonp
from tremppi.configure import configure

# options and system configure
parser = ArgumentParser(description='Initiate a TREMPPI project.')
parser.add_argument('--path', help='specify the browsing location.')
parser.add_argument('--port', help='number of the port to run the browser on')
parser.add_argument('--nopen', help='if set, do not open the browser', action='store_true')

args = parser.parse_args()
system_init(argv[0], args)

if __name__ == "__main__":
    # Execute the server itself.
    if args.port is not None:
        port = args.port
    else:
        port = default_port

    # Start the web server
    app = create_app()
    app.run(port=int(port), debug=True)

    if args.nopen is False:
        webbrowser.open("http://localhost:" + port + "/" + project_path)
