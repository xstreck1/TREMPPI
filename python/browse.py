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

import webbrowser
from argparse import ArgumentParser
from sys import argv
from tremppi.browse_app import create_app
from tremppi.header import system_init, default_port

if __name__ == "__main__":
    # options and system configure
    parser = ArgumentParser(description='Initiate a TREMPPI project.')
    parser.add_argument('--path', help='specify the browsing location.')
    parser.add_argument('--port', help='number of the port to run the browser on')
    parser.add_argument('--nopen', help='if set, do not open the browser', action='store_true')

    args = parser.parse_args()
    system_init(argv[0], args)

    # Execute the server itself.
    if args.port is not None:
        port = args.port
    else:
        port = default_port

    # Start web server
    app = create_app()
    app.run(port=int(port), debug=True)

    if args.nopen is False:
        webbrowser.open("http://localhost:" + port + "/" + project_path)
