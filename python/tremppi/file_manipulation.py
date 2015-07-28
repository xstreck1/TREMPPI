from cmath import log
import shutil
import errno

from tempfile import mkstemp
from shutil import move
from os import remove, close
import re
import json


def replace(file_path, pattern, subst):
    # Create temp file
    fh, abs_path = mkstemp()
    new_file = open(abs_path,'w')
    old_file = open(file_path)
    for line in old_file:
        new_file.write(line.replace(pattern, subst))
    # close temp file
    new_file.close()
    close(fh)
    old_file.close()
    # Remove original file
    remove(file_path)
    # Move new file
    move(abs_path, file_path)


def replace_regex(file_path, pattern, subst):
    # Create temp file
    fh, abs_path = mkstemp()
    new_file = open(abs_path,'w')
    old_file = open(file_path)
    regex = re.compile(pattern)
    for line in old_file:
        new_file.write(regex.sub(subst, line))
    # close temp file
    new_file.close()
    close(fh)
    old_file.close()
    # Remove original file
    remove(file_path)
    # Move new file
    move(abs_path, file_path)


def copyanything(src, dst):
    try:
        shutil.copytree(src, dst)
    except OSError as exc:
        if exc.errno == errno.ENOTDIR:
            shutil.copy(src, dst)
        else:
            raise

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