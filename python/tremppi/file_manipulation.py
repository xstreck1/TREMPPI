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

import errno
import json
import os
import os.path
import re
import shutil
import tempfile


def replace(file_path, pattern, subst):
    # Create temp file
    fh, abs_path = tempfile.mkstemp()
    new_file = open(abs_path,'w')
    old_file = open(file_path)
    for line in old_file:
        new_file.write(line.replace(pattern, subst))
    # close temp file
    new_file.close()
    os.close(fh)
    old_file.close()
    # Remove original file
    os.remove(file_path)
    # Move new file
    shutil.move(abs_path, file_path)


def replace_regex(file_path, pattern, subst):
    # Create temp file
    fh, abs_path = tempfile.mkstemp()
    new_file = open(abs_path,'w')
    old_file = open(file_path)
    regex = re.compile(pattern)
    for line in old_file:
        new_file.write(regex.sub(subst, line))
    # close temp file
    new_file.close()
    os.path.close(fh)
    old_file.close()
    # Remove original file
    os.path.remove(file_path)
    # Move new file
    shutil.move(abs_path, file_path)


def copyanything(src, dst):
    try:
        shutil.copytree(src, dst)
    except OSError as exc:
        if exc.errno == errno.ENOTDIR:
            shutil.copy(src, dst)
        else:
            raise Exception("copyanything failed")


def read_jsonp(filename):
    with open(filename, "r") as file:
        data = file.read().replace('\n', '')
        json_start = 0
        if data.find("{") == -1:
            json_start = data.find("[")
        elif data.find("[") == -1:
            json_start = data.find("{")
        else:
            json_start = min(data.find("{"), data.find("["))
        header = data[0:json_start]
        data = data[json_start:-1]
        return header, json.loads(data)


def write_jsonp(filename, header, data):
    with open(filename, "w") as file:
        file.seek(0)
        file.write(header)
        file.write(json.dumps(data))
        file.write(';')


def path_is_parent(parent_path, child_path):
    parent_path = os.path.abspath(parent_path)
    child_path = os.path.abspath(child_path)

    # Compare the common path of the parent and child path with the common path of just the parent path. Using the commonpath method on just the parent path will regularise the path name in the same way as the comparison that deals with both paths, removing any trailing path separator
    return os.path.commonprefix([parent_path, child_path]) == parent_path