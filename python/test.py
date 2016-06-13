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
from os.path import join
from tremppi.header import system_init, system
from tremppi.select import select
from tremppi.configure import configure

if __name__ == "__main__":
    # define options
    parser = argparse.ArgumentParser(description='Update a TREMPPI project.')
    parser.add_argument('--path', help='specify the location to update.')
    args = parser.parse_args()
    system_init(sys.argv[0], args)

    print(select(join(system.HOME_PATH, 'python', 'test', join('test','select.json')), False) + " : " + select(join(system.HOME_PATH, 'python', 'test', 'select.json'), True))
    configure(system.HOME_PATH + "/javascript/data", "select")
    configure(system.HOME_PATH + "/javascript/data", "properties")
    configure(system.HOME_PATH + "/javascript/data", "quantitative")
    configure(system.HOME_PATH + "/javascript/data", "qualitative")
