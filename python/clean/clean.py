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
import os
import argparse
import shutil
from os.path import join, dirname, abspath, exists
if __name__ == "__main__":
    sys.path.append(dirname(dirname(abspath(sys.argv[0]))))
from tremppi.file_manipulation import copyanything
from tremppi.project_files import generate_data
from tremppi.header import data_folder, configure_filename, system_init, system

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Clean data from a TREMPPI project')
    parser.add_argument('--path', help='specify the location to clean.')
    args = parser.parse_args()
    system_init(sys.argv[0], args)
    if not exists(join(system.DEST_PATH, configure_filename)):
        raise Exception('The target folder ' + system.DEST_PATH + ' does not seem to be a TREMPPI project folder. The ' + configure_filename + ' is missing.')
    else:
        shutil.rmtree(join(system.DEST_PATH, data_folder))
        generate_data(join(system.DEST_PATH, data_folder))