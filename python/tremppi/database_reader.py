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


def read_components(conn):
    return conn.execute('SELECT Name, MaxActivity FROM Components ORDER BY Name').fetchall()


def read_regulations(conn):
    return conn.execute('SELECT DISTINCT Source, Threshold, Target FROM Regulations ORDER BY Source, Target, Threshold').fetchall()


def component_regulators_list(conn):
    result = {}
    cursor = conn.execute('SELECT DISTINCT Target, Source FROM Regulations ORDER BY Target, Source')
    for row in cursor:
        if str(row[0]) not in result:
            result[str(row[0])] = []
        result[str(row[0])].append(str(row[1]))
    return result

