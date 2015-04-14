__author__ = 'adams_000'
import sys
import os
import argparse
import shutil
from os.path import join, dirname, abspath
sys.path.append(dirname(dirname(abspath(sys.argv[0]))))
from tremppi.file_manipulation import copyanything, replace, normal_paths

# define options
parser = argparse.ArgumentParser(description='Update a TREMPPI project.')
parser.add_argument('--path', help='specify the location to update.')
args = parser.parse_args()

EXEC_PATH, BIN_PATH, HOME_PATH, DEST_PATH = normal_paths(sys.argv[0], args)

folders = ["common", "libs", "properties", "editor", "select", "witness", "interact", "function", "summary", "qualitative"]
for folder in folders:
    source = join(join(HOME_PATH, os.path.normpath("javascript/")), folder)
    destination = join(DEST_PATH, folder)
    if os.path.exists(destination):
        shutil.rmtree(destination)
    copyanything(source, destination)

files = ["properties", "editor", "select", "witness", "interact", "function", "summary", "qualitative"]
for file in files:
    shutil.copy(join(HOME_PATH, os.path.normpath("javascript/" + file + ".html")), DEST_PATH)