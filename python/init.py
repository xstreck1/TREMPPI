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
import sys

from tremppi.header import system_init, system
from tremppi.project_files import tremppi_init

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Initiate a TREMPPI project.')
    parser.add_argument('--path', help='specify the location where the file gets created.')
    parser.add_argument('--server', help='informs the script that it is called from a server.', action='store_true')
    parser.add_argument('name', help='name of the newly created model')
    args = parser.parse_args()

    system_init(sys.argv[0], args)
    tremppi_init(system.DEST_PATH, args.name)