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
import shutil
from os.path import join, dirname, abspath, exists

if __name__ == "__main__":
    sys.path.append(dirname(dirname(abspath(sys.argv[0]))))
from tremppi.file_manipulation import copyanything
from tremppi.project_files import generate_data
from tremppi.header import folders, widgets, source_folder, data_folder, configure_filename, system_init, system, version


def init(name):
    DEST_CONTENT = join(system.DEST_PATH, name)
    if exists(DEST_CONTENT):
        raise Exception('The destination folder ' + DEST_CONTENT + ' already exists, aborting.')

    # copy the data
    for folder in folders:
        source = join(join(system.HOME_PATH, source_folder), folder)
        destination = join(DEST_CONTENT, folder)
        copyanything(source, destination)

    for file in widgets:
        shutil.copy(join(system.HOME_PATH, source_folder, file + ".html"), DEST_CONTENT)

    shutil.copy(join(system.HOME_PATH, source_folder, "logo.png"), DEST_CONTENT)
    shutil.copy(join(system.HOME_PATH, source_folder, "favicon.ico"), DEST_CONTENT)

    # make the data directory
    generate_data(join(DEST_CONTENT, data_folder))

    # create the configure data
    with open(join(DEST_CONTENT, 'configure.js'), 'w+') as setup:
        setup.write('tremppi.setup = { ' +
                    '\t"project_name": "' + name + '",\n' +
                    '\t"version": "' + version + '"\n' +
                    '};'
                    )

    open(join(DEST_CONTENT, 'log.txt'), 'w+')

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Initiate a TREMPPI project.')
    parser.add_argument('--path', help='specify the location where the file gets created.')
    parser.add_argument('name', help='name of the newly created model')
    args = parser.parse_args()

    system_init(sys.argv[0], args)
    init(args.name)