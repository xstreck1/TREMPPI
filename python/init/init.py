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
		
def replace(file_path, pattern, subst):
    #Create temp file
    fh, abs_path = tempfile.mkstemp()
    new_file = open(abs_path,'w')
    old_file = open(file_path)
    for line in old_file:
        new_file.write(line.replace(pattern, subst))
    #close temp file
    new_file.close()
    os.close(fh)
    old_file.close()
    #Remove original file
    os.remove(file_path)
    #Move new file
    shutil.move(abs_path, file_path)

#define options
parser = argparse.ArgumentParser(description='Initiate a TREMPPI project.')
parser.add_argument('--dest', help='specify the location where the file gets created.')
parser.add_argument('name', help='name of the newly created model')
args = parser.parse_args()
print(sys.argv[0])

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
new_model = args.name + ".js"
replace(os.path.join(DEST_PATH, 'viewer.html'), 'basic_model.js', new_model)
os.rename(os.path.join(DEST_PATH, 'basic_model.js'), os.path.join(DEST_PATH, new_model))