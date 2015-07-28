from cmath import log
import shutil
import errno

from tempfile import mkstemp
from shutil import move, rmtree
from os import remove, close, makedirs
from os.path import join, isfile, exists
from tremppi.header import configure_filename, widgets
import re
import json


# make sure all the data are present
def generate_data(data_path):
    if not exists(data_path):
        makedirs(data_path)
    for widget in widgets:
        # main json
        json_filename = join(data_path, widget + '.json')
        if not isfile(json_filename):
            with open(json_filename, 'w+') as json_file:
                json.dump({}, json_file)
        # config js
        js_filename = join(data_path, widget + '.js')
        if not isfile(js_filename):
            open(js_filename, 'w+').close()
        if not exists(join(data_path, widget)):
            makedirs(join(data_path, widget))


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