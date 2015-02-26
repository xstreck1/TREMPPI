__author__ = 'adams_000'
import sys
import os
import argparse
import shutil
import re
from os.path import join, dirname

# define options
parser = argparse.ArgumentParser(description='Update a TREMPPI project.')
parser.add_argument('--path', help='specify the location to update.')
args = parser.parse_args()

# find paths & load common functions
sys.path.append(dirname(dirname(os.path.abspath(sys.argv[0]))))
from tremppi_common.file_manipulation import copyanything, replace, normal_paths

EXEC_PATH, BIN_PATH, HOME_PATH, DEST_PATH = normal_paths(sys.argv[0], args)


folder_types = ["common", "properties", "editor", "select", "witness", "interact", "function"]
for type in folder_types:
    source = join(join(HOME_PATH, os.path.normpath("javascript/")), type)
    destination = join(DEST_PATH, type)
    if os.path.exists(destination):
        shutil.rmtree(destination)
    copyanything(source, destination)


shutil.copy(join(HOME_PATH, os.path.normpath("javascript/editor.html")), DEST_PATH)
shutil.copy(join(HOME_PATH, os.path.normpath("javascript/properties.html")), DEST_PATH)
shutil.copy(join(HOME_PATH, os.path.normpath("javascript/select.html")), DEST_PATH)
files = os.listdir(DEST_PATH)
report_types = ["witness", "interact", "function"]
for type in report_types:
    type_files = [value for value in files if re.match(type + "_.*html", value)]
    for file in type_files:
        shutil.copy(join(HOME_PATH, os.path.normpath("javascript/" + type + ".html")), join(DEST_PATH, file))