import sys
import os
import argparse
import shutil
import errno
import tempfile

def copyanything(src, dst):
    try:
        shutil.copytree(src, dst)
    except OSError as exc: # python >2.5
        if exc.errno == errno.ENOTDIR:
            shutil.copy(src, dst)
        else: raise
		
#define options
parser = argparse.ArgumentParser(description='Initiate a TREMPPI project.')
parser.add_argument('--dest', help='specify the location where the file gets created.')
parser.add_argument('name', help='name of the newly created model')
args = parser.parse_args()

# find paths
EXEC_PATH = os.path.dirname(os.path.abspath(sys.argv[0]))
BIN_PATH = os.path.abspath(os.path.join(EXEC_PATH, os.path.normpath("./../../bin")))
HOME_PATH = os.path.abspath(os.path.join(EXEC_PATH, os.path.normpath("./../../")))
if args.dest != None:
	DEST_PATH = os.path.join(os.path.normpath(args.dest), args.name)
else:
	DEST_PATH = os.path.join(os.getcwd(), args.name)
SRC_PATH = os.path.join(HOME_PATH, os.path.normpath("javascript/viewer/"))

# copy data and rename content
if os.path.exists(DEST_PATH):
	shutil.rmtree(os.path.join(DEST_PATH, os.path.normpath("./")))
copyanything(SRC_PATH, os.path.join(DEST_PATH, os.path.normpath("./")))