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

types = ["properties", "editor", "select", "witness", "interact", "function", "summary"]
for type in types:
    source = join(join(HOME_PATH, os.path.normpath("javascript/")), type)
    destination = join(DEST_PATH, type)
    if os.path.exists(destination):
        shutil.rmtree(destination)
    copyanything(source, destination)
    shutil.copy(join(HOME_PATH, os.path.normpath("javascript/" + type + ".html")), DEST_PATH)