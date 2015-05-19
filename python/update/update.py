__author__ = 'adams_000'
import sys
import os
import argparse
import shutil
from os.path import join, dirname, abspath
sys.path.append(dirname(dirname(abspath(sys.argv[0]))))
from tremppi.file_manipulation import copyanything, replace, normal_paths
from tremppi.header import folders, widgets, source_folder

# define options
parser = argparse.ArgumentParser(description='Update a TREMPPI project.')
parser.add_argument('--path', help='specify the location to update.')
args = parser.parse_args()

EXEC_PATH, BIN_PATH, HOME_PATH, DEST_PATH = normal_paths(sys.argv[0], args)

for folder in folders:
    source = join(join(HOME_PATH, source_folder), folder)
    destination = join(DEST_PATH, folder)
    if os.path.exists(destination):
        shutil.rmtree(destination)
    copyanything(source, destination)

for file in widgets:
    shutil.copy(join(HOME_PATH, source_folder, file + ".html"), DEST_PATH)

shutil.copy(join(HOME_PATH, source_folder, "favicon.ico"), DEST_PATH)