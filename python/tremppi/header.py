__author__ = 'adams_000'

default_port = "8080"
configure_filename = "configure.js"
projects_filename = "projects.js"
last_page_filename = "last_page.txt"
database_file = 'database.sqlite'
source_folder = 'javascript'
data_folder = 'data'
widgets = ['index', 'properties', 'editor', 'select', 'witness', 'regulations', 'correlations', 'quantitative', 'qualitative', 'tools']
folders = ['common', 'libs', 'index', 'properties', 'editor', 'select', 'witness', 'regulations', 'correlations',  'quantitative', 'qualitative', 'tools']
version = "0.4.1"

from os.path import dirname, abspath, join, normpath, basename, isfile
from os import getcwd


class System:
    EXEC_PATH = ''
    BIN_PATH = ''
    HOME_PATH = ''
    DEST_PATH = ''
system = System()

def system_init(exec_path, args):
    global system
    system.EXEC_PATH = dirname(abspath(exec_path))
    if basename(exec_path) == 'tremppi' or basename(exec_path) == 'tremppi.exe':
        system.BIN_PATH = abspath(dirname(exec_path))
        system.HOME_PATH = system.BIN_PATH
    elif isfile(join(dirname(dirname(dirname(exec_path))), 'tremppi')) or isfile(join(dirname(dirname(dirname(exec_path))), 'tremppi.exe')):
        system.BIN_PATH = abspath(dirname(dirname(dirname(exec_path))))
        system.HOME_PATH = system.BIN_PATH
    elif isfile(join(dirname(dirname(dirname(exec_path))), join('Release','tremppi.exe'))):
        system.BIN_PATH = abspath(join(dirname(dirname(dirname(exec_path))), 'Release'))
        system.HOME_PATH = dirname(system.BIN_PATH)
    else:
        raise Exception('TREMPPI binary not found')
    if args.path is not None:
        system.DEST_PATH = normpath(args.path)
    else:
        system.DEST_PATH = getcwd()
    return system
