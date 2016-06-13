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

import argparse
import os
import sys
from tremppi.server_app import create_app
from tremppi.header import system_init, default_port, system, server_folder

if __name__ == "__main__":
    # options and system configure
    parser = argparse.ArgumentParser(description='Start a tremppi server.')
    parser.add_argument('--path', help='the path argument is ignored')
    parser.add_argument('--port', help='number of the port to run the browser on')
    parser.add_argument('--debug', help='if set, run debug', action='store_false')

    args = parser.parse_args()
    system_init(sys.argv[0], args)

    # set the server to the fixed location
    system.DEST_PATH = os.path.join(system.HOME_PATH, server_folder)
    if not os.path.isdir(system.DEST_PATH):
        raise Exception('The server folder does not exist.')

    if args.port is not None:
        port = args.port
    else:
        port = default_port

    # Start the web server
    app = create_app()
    app.run(port=int(port), debug=args.debug, host='192.168.1.8')
