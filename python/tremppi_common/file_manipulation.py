import shutil
import errno

from tempfile import mkstemp
from shutil import move
from os import remove, close
from os.path import dirname, abspath, normpath, join

def normal_paths(exec_path):
    EXEC_PATH = dirname(abspath(exec_path))
    BIN_PATH = abspath(join(EXEC_PATH, normpath("./../../bin")))
    HOME_PATH = abspath(join(EXEC_PATH, normpath("./../../")))
    return (EXEC_PATH, BIN_PATH, HOME_PATH)

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
    except OSError as exc:
        if exc.errno == errno.ENOTDIR:
            shutil.copy(src, dst)
        else:
            raise

