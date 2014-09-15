import sys
import os
import argparse
import shutil
from os.path import join as pjoin

# define options
parser = argparse.ArgumentParser(description='Initiate a TREMPPI project.')
parser.add_argument('--dest', help='specify the location where the file gets created.')
parser.add_argument('name', help='name of the newly created model')
args = parser.parse_args()

# find paths
sys.path.append(os.path.abspath(pjoin(sys.argv[0], "../../")))
from tremppi_common.file_manipulation import normal_paths, copyanything, replace
EXEC_PATH, BIN_PATH, HOME_PATH = normal_paths(sys.argv[0])
if args.dest is not None:
    DEST_PATH = pjoin(os.path.normpath(args.dest), args.name)
else:
    DEST_PATH = pjoin(os.getcwd(), args.name)
SRC_PATH = os.path.join(HOME_PATH, os.path.normpath("javascript/editor/"))

# copy data and rename content
if os.path.exists(DEST_PATH):
    shutil.rmtree(pjoin(DEST_PATH, os.path.normpath("./")))
copyanything(SRC_PATH, pjoin(DEST_PATH, os.path.normpath("./")))

MODEL_NAME = os.path.split(DEST_PATH)[1]
replace(os.path.join(DEST_PATH, "editor.html"), "dummy_model_name", MODEL_NAME)
print(DEST_PATH)