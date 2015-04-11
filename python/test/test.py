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
from tremppi.tabularize import tabularize

# define options
parser = argparse.ArgumentParser(description='Update a TREMPPI project.')
parser.add_argument('--path', help='specify the location to update.')
args = parser.parse_args()
EXEC_PATH, BIN_PATH, HOME_PATH, DEST_PATH = normal_paths(sys.argv[0], args)

print(select(HOME_PATH + "/javascript/select.json"))
tabularize(HOME_PATH + "/javascript/database.sqlite", HOME_PATH + "/javascript/select.json")
tabularize("C:/Users/adams_000/MODELS/Tremppi/EGFR_article/database.sqlite", "C:/Users/adams_000/MODELS/Tremppi/EGFR_article/select.json")