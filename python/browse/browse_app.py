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

from flask import Flask, request, redirect

from initiate_projects import mk_usr_proj
from query_responses import do_get, do_post, InvalidUsage

from tremppi.header import system

# Setup Flask app and app.config
def create_app():
    app = Flask(__name__)

    # ROUTES
    @app.route('/')
    def home_page():
        return redirect(mk_usr_proj(system.DEST_PATH))

    @app.route('/<path:path>', methods=['GET', 'POST'])
    def serve(path):
        if request.method == 'GET':
            return do_get(request.path[1:])
        elif request.method == 'POST':
            return do_post(request.path[1:])
        else:
            raise MethodNotAllowed()

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