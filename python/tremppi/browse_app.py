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

from flask import Flask, request, redirect, jsonify

from .initiate_projects import mk_usr_proj
from .server_errors import InvalidUsage, MethodNotAllowed, Conflict
from .header import system
from .query_responses import do_post, do_get
from .tool_manager import ToolManager

# Setup Flask app and app.config
def create_app():
    app = Flask(__name__)

    def projects_path():
        return system.DEST_PATH

    app.projects_path = projects_path

    tool_manager = ToolManager(True)

    def get_tool_manager():
        return tool_manager
    app.get_tool_manager = get_tool_manager

    # ROUTES
    @app.route('/')
    def home_page():
        return redirect(mk_usr_proj(system.DEST_PATH))

    @app.route('/exec_type.js', methods=['GET'])
    def exec_type():
        return 'tremppi.exec_type = "browse"'


    @app.route('/exit', methods=['POST'])
    def exit_tremppi():
        func = request.environ.get('werkzeug.server.shutdown')
        if func is None:
            raise RuntimeError('Not running with the Werkzeug Server')
        func()
        return 'exit successful'

    @app.route('/<path:path>', methods=['GET', 'POST'])
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

    return app
