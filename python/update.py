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
from os.path import join
from tremppi.file_manipulation import copyanything, read_jsonp, write_jsonp
from tremppi.project_files import generate_data
from tremppi.header import folders, widgets, source_folder, system, system_init, data_folder, version, configure_filename

if __name__ == "__main__":
    # define options
    parser = argparse.ArgumentParser(description='Update a TREMPPI project.')
    parser.add_argument('--path', help='specify the location to update.')
    args = parser.parse_args()

    system_init(sys.argv[0], args)
    for folder in folders:
        source = join(join(system.HOME_PATH, source_folder), folder)
        destination = join(system.DEST_PATH, folder)
        if os.path.exists(destination):
            shutil.rmtree(destination)
        copyanything(source, destination)

    for file in widgets:
        shutil.copy(join(system.HOME_PATH, source_folder, file + ".html"), system.DEST_PATH)
        generate_data(join(system.DEST_PATH, data_folder))

    # update version
    file_path = join(system.DEST_PATH, configure_filename)
    header, data = read_jsonp(file_path)
    data['version'] = version
    write_jsonp(file_path, header, data)

    shutil.copy(join(system.HOME_PATH, source_folder, "favicon.ico"), system.DEST_PATH)
    shutil.copy(join(system.HOME_PATH, source_folder, "logo.png"), system.DEST_PATH)