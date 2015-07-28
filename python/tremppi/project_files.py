__author__ = 'adams_000'
import json
from os import listdir, makedirs
from os.path import join, exists, isdir, isfile
from shutil import rmtree
from tremppi.header import configure_filename, projects_filename, widgets
from tremppi.file_manipulation import read_jsonp, write_jsonp

# make sure all the data are present
def generate_data(data_path):
    if not exists(data_path):
        makedirs(data_path)
    for widget in widgets:
        # main json
        json_filename = join(data_path, widget + '.json')
        if not isfile(json_filename):
            with open(json_filename, 'w+') as json_file:
                if (widget == 'select'):
                    json.dump({}, json_file)
                else:
                    json.dump({
                        "records": [{
                            "name": "all",
                            "recid": 0,
                            "select": True
                        }]},
                        json_file)
        # config js
        js_filename = join(data_path, widget + '.js')
        if not isfile(js_filename):
            open(js_filename, 'w+').close()
        if not exists(join(data_path, widget)):
            makedirs(join(data_path, widget))


# return subdirectories that contain configure.js
def list_projects(projects_path):
    result = []
    subdirs = [name for name in listdir(projects_path) if isdir(join(projects_path, name))]
    for subdir in subdirs:
        if exists(join(projects_path, subdir, configure_filename)):
            result.append(subdir)
    return result


def write_projects(projects_path):
    header, data = read_jsonp(join(projects_path, projects_filename))
    data = list_projects(projects_path)
    write_jsonp(join(projects_path, projects_filename), header, data)


def delete_project(name):
    print(name)
    if not isfile(join(name, configure_filename)):
        raise name + " seems not to be a TREMPPI project"
    else:
        rmtree(name)


def save_file(store_path, data):
    with open(store_path, 'w') as file:
        file.write(data.decode())

def get_log(log_path):
    with open(log_path, 'r') as file:
        return file.read()

def get_path_level(path): #data files have level -1, project files 0, projects 1
    return 1 - path.count("/")