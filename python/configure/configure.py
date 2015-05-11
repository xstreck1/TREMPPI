__author__ = 'adams_000'
import sys
import argparse
import os
import json
from os.path import join, dirname, abspath, exists
sys.path.append(dirname(dirname(abspath(sys.argv[0]))))
from tremppi.header import data_folder, widgets
from tremppi.file_manipulation import normal_paths
from tremppi.tabularize import tabularize

# define options
parser = argparse.ArgumentParser(description='Create configurations for the HTML project browser.')
parser.add_argument('--path', help='specify the location to update.')
args = parser.parse_args()

EXEC_PATH, BIN_PATH, HOME_PATH, DEST_PATH = normal_paths(sys.argv[0], args)
DATA_PATH = join(DEST_PATH, data_folder)

# create the empty configuration files
for widget in widgets:
    js_filename = join(DATA_PATH, widget + '.js')
    open(js_filename, 'w+').close()


tabularize(join(DATA_PATH, 'database.sqlite'), 'select.js')

for widget in ["qualitative", "quantitative", "interact", "function"]:
    files = []
    widget_dir = join(DATA_PATH, widget)
    if not exists(widget_dir):
        os.makedirs(widget_dir)
    else:
        for file in os.listdir(widget_dir):
            if file.endswith(".json"):
                files.append(file)
    with open(join(DATA_PATH, widget + '.js'), 'w+') as js_file:
        js_file.write('tremppi.' + widget + '.file_list = ')
        json.dump(files, files)
        js_file.write(';')