__author__ = 'adams_000'
import sys
import argparse

from tremppi.file_manipulation import  normal_paths
from tremppi.select import select
from configure.configure import configure

# define options
parser = argparse.ArgumentParser(description='Update a TREMPPI project.')
parser.add_argument('--path', help='specify the location to update.')
args = parser.parse_args()
EXEC_PATH, BIN_PATH, HOME_PATH, DEST_PATH = normal_paths(sys.argv[0], args)

print(select("C:/Users/adams_000/TREMPPI/projects/HSB_2015/data/select.json", False) + " : " + select("C:/Users/adams_000/TREMPPI/projects/HSB_2015/data/select.json", True))
configure(HOME_PATH + "/javascript/data", "select")
configure(HOME_PATH + "/javascript/data", "properties")
configure(HOME_PATH + "/javascript/data", "quantitative")
configure(HOME_PATH + "/javascript/data", "qualitative")
