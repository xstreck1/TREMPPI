__author__ = 'adams_000'
import sys
import os
import argparse
import shutil
import re
import json
from os.path import join, dirname

# define options
parser = argparse.ArgumentParser(description='Update a TREMPPI project.')
parser.add_argument('--path', help='specify the location to update.')
args = parser.parse_args()

# find paths & load common functions
sys.path.append(dirname(dirname(os.path.abspath(sys.argv[0]))))
from tremppi.file_manipulation import copyanything, replace, normal_paths

EXEC_PATH, BIN_PATH, HOME_PATH, DEST_PATH = normal_paths(sys.argv[0], args)

from tremppi.select import selection_from_file

print(selection_from_file(HOME_PATH + "/javascript/select.json"))