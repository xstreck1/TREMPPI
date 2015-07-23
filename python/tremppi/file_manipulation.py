import shutil
import errno

from tempfile import mkstemp
from shutil import move, rmtree
from os import remove, close, getcwd
from os.path import dirname, abspath, normpath, join, isfile
from tremppi.header import configure_filename
import re
import json


def normal_paths(exec_path, args):
    EXEC_PATH = dirname(abspath(exec_path))
    BIN_PATH = abspath(join(EXEC_PATH, normpath("./../../bin")))
    HOME_PATH = abspath(join(EXEC_PATH, normpath("./../../")))
    if args.path is not None:
        DEST_PATH = normpath(args.path)
    else:
        DEST_PATH = getcwd()
    return EXEC_PATH, BIN_PATH, HOME_PATH, DEST_PATH


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


def save_file(store_path, data):
    with open(store_path, 'w') as file:
        file.write(data.decode())

def get_log(log_path):
    with open(log_path, 'r') as file:
        return file.read()

def delete_project(name):
    if not isfile(join(name, configure_filename)):
        raise name + " seems not to be a TREMPPI project"
    else:
        rmtree(name)

def get_path_level(path): #data files have level -1, project files 0, projects 1
    return 1 - path.count("/")

def read_jsonp(filename):
    with open(filename, "r") as file:
        data = file.read().replace('\n', '')
        header = data[0:data.find("{")]
        data = data[data.find("{"):-1]
        return header, json.loads(data)

def write_jsonp(filename, header, data):
    with open(filename, "w") as file:
        file.seek(0)
        file.write(header)
        file.write(json.dumps(data))
        file.write(';')