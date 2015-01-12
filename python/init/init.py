import sys
import os
import argparse
import shutil
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

# delete original if present
DEST_CONTENT = join(DEST_PATH, args.name)
if os.path.exists(DEST_CONTENT):
    shutil.rmtree(DEST_CONTENT)

# copy the data
copyanything(join(HOME_PATH, os.path.normpath("javascript/common/")), join(DEST_CONTENT, "common"))
copyanything(join(HOME_PATH, os.path.normpath("javascript/browse/")), join(DEST_CONTENT, "browse"))
copyanything(join(HOME_PATH, os.path.normpath("javascript/properties/")), join(DEST_CONTENT, "properties"))
copyanything(join(HOME_PATH, os.path.normpath("javascript/editor/")), join(DEST_CONTENT, "editor"))
copyanything(join(HOME_PATH, os.path.normpath("javascript/select/")), join(DEST_CONTENT, "select"))
shutil.copy(join(HOME_PATH, os.path.normpath("javascript/editor.html")), DEST_CONTENT)
shutil.copy(join(HOME_PATH, os.path.normpath("javascript/browse.html")), DEST_CONTENT)
shutil.copy(join(HOME_PATH, os.path.normpath("javascript/properties.html")), DEST_CONTENT)
shutil.copy(join(HOME_PATH, os.path.normpath("javascript/select.html")), DEST_CONTENT)
# data files are not copied - create new
open(join(DEST_CONTENT, "editor.json"), 'w').write("{nodes: [], edges:[]}")
open(join(DEST_CONTENT, "properties.json"), 'w').write("[]")
open(join(DEST_CONTENT, "select.json"), 'w').write("[]")
open(join(DEST_CONTENT, "browse.json"), 'w').write("{}")