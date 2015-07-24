__author__ = 'adams_000'
import sys
import os
import argparse
import shutil
from os.path import join, dirname, abspath
if __name__ == "__main__":
    sys.path.append(dirname(dirname(abspath(sys.argv[0]))))
from tremppi.file_manipulation import copyanything
from tremppi.header import folders, widgets, source_folder, system, system_init

# define options
parser = argparse.ArgumentParser(description='Update a TREMPPI project.')
parser.add_argument('--path', help='specify the location to update.')
args = parser.parse_args()

system_init(sys.argv[0], args)
for folder in folders:
    source = join(join(system.HOME_PATH, source_folder), folder)
    destination = join(system.DEST_PATH, folder)
    if os.path.exists(destination):
        shutil.rmtree(destination)
    copyanything(source, destination)

for file in widgets:
    shutil.copy(join(system.HOME_PATH, source_folder, file + ".html"), system.DEST_PATH)

shutil.copy(join(system.HOME_PATH, source_folder, "favicon.ico"), system.DEST_PATH)