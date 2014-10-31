import sys
import os
import argparse
import shutil
from os.path import join, dirname

# define options
parser = argparse.ArgumentParser(description='Initiate a TREMPPI project.')
parser.add_argument('--dest', help='specify the location where the file gets created.')
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
shutil.copy(join(HOME_PATH, os.path.normpath("javascript/editor.html")), DEST_CONTENT)
shutil.copy(join(HOME_PATH, os.path.normpath("javascript/browse.html")), DEST_CONTENT)
shutil.copy(join(HOME_PATH, os.path.normpath("javascript/properties.html")), DEST_CONTENT)

MODEL_NAME = os.path.split(DEST_PATH)[1]
replace(join(DEST_CONTENT, "editor.html"), "dummy_model_name", MODEL_NAME)