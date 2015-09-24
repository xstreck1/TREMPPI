__author__ = 'adams_000'
import sys
import argparse
from os.path import join

from tremppi.header import system_init, system
from tremppi.select import select
from tremppi.configure import configure

# define options
parser = argparse.ArgumentParser(description='Update a TREMPPI project.')
parser.add_argument('--path', help='specify the location to update.')
args = parser.parse_args()
system_init(sys.argv[0], args)

print(select(join(system.HOME_PATH, 'python', 'test', 'select.json'), False) + " : " + select(join(system.HOME_PATH, 'python', 'test', 'select.json'), True))
configure(system.HOME_PATH + "/javascript/data", "select")
configure(system.HOME_PATH + "/javascript/data", "properties")
configure(system.HOME_PATH + "/javascript/data", "quantitative")
configure(system.HOME_PATH + "/javascript/data", "qualitative")
