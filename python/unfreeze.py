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
import os.path
from tremppi.header import data_folder, configure_filename, system_init, system, model_file
from tremppi.clean import clean
from tremppi.configure import configure

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Erase the dynamic labels and the values for the properties.')
    parser.add_argument('--path', help='specify the location to unfreeze')
    parser.add_argument('--server', help='informs the script that it is called from a server.', action='store_true')
    args = parser.parse_args()
    system_init(sys.argv[0], args)
    DATA_PATH = os.path.join(system.DEST_PATH, data_folder)
    if not os.path.exists(os.path.join(system.DEST_PATH, configure_filename)):
        raise Exception('The target folder ' + system.DEST_PATH + ' does not seem to be a TREMPPI project folder. The ' + configure_filename + ' is missing.')
    else:
        clean(DATA_PATH, 'properties')
        configure(DATA_PATH, 'properties')
        configure(DATA_PATH, 'tools')