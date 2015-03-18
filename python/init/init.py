import sys
import os
import argparse
import shutil
from os.path import join, dirname, abspath

print(sys.path)

sys.path.append(dirname(dirname(abspath(sys.argv[0]))))
from tremppi.file_manipulation import copyanything, replace, normal_paths

# define options
parser = argparse.ArgumentParser(description='Initiate a TREMPPI project.')
parser.add_argument('--path', help='specify the location where the file gets created.')
parser.add_argument('name', help='name of the newly created model')
args = parser.parse_args()

# find paths & load tremppi functions
EXEC_PATH, BIN_PATH, HOME_PATH, DEST_PATH = normal_paths(sys.argv[0], args)

# delete original if present
DEST_CONTENT = join(DEST_PATH, args.name)
if os.path.exists(DEST_CONTENT):
    shutil.rmtree(DEST_CONTENT)

# copy the data
copyanything(join(HOME_PATH, os.path.normpath("javascript/libs/")), join(DEST_CONTENT, "libs"))
copyanything(join(HOME_PATH, os.path.normpath("javascript/common/")), join(DEST_CONTENT, "common"))

copyanything(join(HOME_PATH, os.path.normpath("javascript/properties/")), join(DEST_CONTENT, "properties"))
copyanything(join(HOME_PATH, os.path.normpath("javascript/editor/")), join(DEST_CONTENT, "editor"))
copyanything(join(HOME_PATH, os.path.normpath("javascript/select/")), join(DEST_CONTENT, "select"))
copyanything(join(HOME_PATH, os.path.normpath("javascript/witness/")), join(DEST_CONTENT, "witness"))
copyanything(join(HOME_PATH, os.path.normpath("javascript/interact/")), join(DEST_CONTENT, "interact"))
copyanything(join(HOME_PATH, os.path.normpath("javascript/function/")), join(DEST_CONTENT, "function"))

shutil.copy(join(HOME_PATH, os.path.normpath("javascript/editor.html")), DEST_CONTENT)
shutil.copy(join(HOME_PATH, os.path.normpath("javascript/properties.html")), DEST_CONTENT)
shutil.copy(join(HOME_PATH, os.path.normpath("javascript/select.html")), DEST_CONTENT)
shutil.copy(join(HOME_PATH, os.path.normpath("javascript/witness.html")), DEST_CONTENT)
shutil.copy(join(HOME_PATH, os.path.normpath("javascript/interact.html")), DEST_CONTENT)
shutil.copy(join(HOME_PATH, os.path.normpath("javascript/function.html")), DEST_CONTENT)
# data files are not copied - create new
open(join(DEST_CONTENT, "editor.json"), 'w').write('{}')
open(join(DEST_CONTENT, "properties.json"), 'w').write('{}')
open(join(DEST_CONTENT, "select.json"), 'w').write('{}')
open(join(DEST_CONTENT, "witness.json"), 'w').write('{}')
open(join(DEST_CONTENT, "interact.json"), 'w').write('{}')
open(join(DEST_CONTENT, "function.json"), 'w').write('{}')