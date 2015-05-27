__author__ = 'adams_000'
import sys
import os
import argparse
import shutil
import re
import json
from os.path import join, dirname
sys.path.append(dirname(dirname(os.path.abspath(sys.argv[0]))))
from tremppi.file_manipulation import  normal_paths
from tremppi.select import select
from configure.configure import configure

# define options
parser = argparse.ArgumentParser(description='Update a TREMPPI project.')
parser.add_argument('--path', help='specify the location to update.')
args = parser.parse_args()
EXEC_PATH, BIN_PATH, HOME_PATH, DEST_PATH = normal_paths(sys.argv[0], args)

print(select(HOME_PATH + "/javascript/data/select.json", False) + " : " + select(HOME_PATH + "/javascript/data/select.json", True))
configure(HOME_PATH + "/javascript/data", "select")
configure(HOME_PATH + "/javascript/data", "properties")
configure(HOME_PATH + "/javascript/data", "quantitative")