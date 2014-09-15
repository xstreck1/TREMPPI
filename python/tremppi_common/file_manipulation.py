import shutil
import errno
import os

from tempfile import mkstemp
from shutil import move
from os import remove, close

def replace(file_path, pattern, subst):
    #Create temp file
    fh, abs_path = mkstemp()
    new_file = open(abs_path,'w')
    old_file = open(file_path)
    for line in old_file:
        new_file.write(line.replace(pattern, subst))
    #close temp file
    new_file.close()
    close(fh)
    old_file.close()
    #Remove original file
    remove(file_path)
    #Move new file
    move(abs_path, file_path)

def copyanything(src, dst):
    try:
        shutil.copytree(src, dst)
    except OSError as exc: # python >2.5
        if exc.errno == errno.ENOTDIR:
            shutil.copy(src, dst)
        else:
            raise

def normal_paths(exec_path):
    EXEC_PATH = os.path.dirname(os.path.abspath(exec_path))
    BIN_PATH = os.path.abspath(os.path.join(EXEC_PATH, os.path.normpath("./../../bin")))
    HOME_PATH = os.path.abspath(os.path.join(EXEC_PATH, os.path.normpath("./../../")))
    return (EXEC_PATH, BIN_PATH, HOME_PATH)