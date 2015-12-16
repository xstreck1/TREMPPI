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

import sys
import argparse
from os.path import join, dirname, abspath
if __name__ == "__main__":
    sys.path.append(dirname(dirname(abspath(sys.argv[0]))))
from tremppi.header import data_folder, widgets, system, system_init
from tremppi.configure import configure

if __name__ == "__main__":
    # define options
    parser = argparse.ArgumentParser(description='Create configurations for the HTML project browser.')
    parser.add_argument('--path', help='specify the location to update.')
    args = parser.parse_args()
    system_init(sys.argv[0], args)
    DATA_PATH = join(system.DEST_PATH, data_folder)
    # generate files
    for widget in widgets:
        configure(DATA_PATH, widget)

