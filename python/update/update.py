__author__ = 'adams_000'
import sys
import os
import argparse
import re
from os.path import join, dirname

# define options
parser = argparse.ArgumentParser(description='Initiate a TREMPPI project.')
parser.add_argument('--path', help='specify the location where the file gets created.')
parser.add_argument('name', help='name of the newly created model')
args = parser.parse_args()

# find paths & load common functions
sys.path.append(dirname(dirname(os.path.abspath(sys.argv[0]))))
from tremppi_common.file_manipulation import copyanything, replace, normal_paths

EXEC_PATH, BIN_PATH, HOME_PATH, DEST_PATH = normal_paths(sys.argv[0], args)

widgets = ["browse", "editor", "properties", "select"]

js_files = os.listdir(".")
js_files = [value for value in files if re.match(".*js", value)]