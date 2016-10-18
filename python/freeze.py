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
import json
import os.path
import sqlite3
from os.path import join
from tremppi.header import data_folder, configure_filename, system_init, system, database_file, properties_file
from tremppi.file_manipulation import read_jsonp
from tremppi.configure import configure

# TODO needs to read the properties from the JSON file and insert into columns and Properties

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Erase all the data of a tremppi project except for the editor')
    parser.add_argument('--path', help='specify the location to freeze')
    args = parser.parse_args()
    system_init(sys.argv[0], args)
    DATA_PATH = join(system.DEST_PATH, data_folder)
    if not os.path.exists(os.path.join(system.DEST_PATH, configure_filename)):
        raise Exception('The target folder ' + system.DEST_PATH + ' does not seem to be a TREMPPI project folder. The ' + configure_filename + ' is missing.')
    else:
        with sqlite3.connect(os.path.join(DATA_PATH, database_file)) as conn:
            conn.execute('DROP TABLE IF EXISTS Properties')
            conn.execute('CREATE TABLE Properties (Name TEXT)')
            properties = []
            with open(os.path.join(DATA_PATH, properties_file), "r") as file:
                property_holder = json.loads(file.read())
                if "records" in property_holder:
                    properties = property_holder["records"]
            for property in properties:
                conn.execute('INSERT INTO Properties (Name) VALUES (\"' + property["name"] + '\")')
        configure(DATA_PATH, 'properties')
        configure(DATA_PATH, 'tools')
