__author__ = 'adams_000'
import sys
import argparse

from tremppi.header import system_init, system
from tremppi.select import select
from configure.configure import configure

# define options
parser = argparse.ArgumentParser(description='Update a TREMPPI project.')
parser.add_argument('--path', help='specify the location to update.')
args = parser.parse_args()
system_init(sys.argv[0], args)

print(select("C:/Users/adams_000/TREMPPI/projects/HSB_2015/data/select.json", False) + " : " + select("C:/Users/adams_000/TREMPPI/projects/HSB_2015/data/select.json", True))
configure(system.HOME_PATH + "/javascript/data", "select")
configure(system.HOME_PATH + "/javascript/data", "properties")
configure(system.HOME_PATH + "/javascript/data", "quantitative")
configure(system.HOME_PATH + "/javascript/data", "qualitative")
