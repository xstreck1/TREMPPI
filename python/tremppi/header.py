__author__ = 'adams_000'

default_port = "8080"
configure_filename = "configure.js"
projects_filename = "projects.js"
database_file = 'database.sqlite'
source_folder = 'javascript'
data_folder = 'data'
widgets = ['index', 'properties', 'editor', 'select', 'witness', 'regulations', 'correlations', 'quantitative', 'qualitative', 'tools']
folders = ['common', 'libs', 'index', 'properties', 'editor', 'select', 'witness', 'regulations', 'correlations',  'quantitative', 'tools']

from os.path import dirname, abspath, join, normpath
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
    system.BIN_PATH = abspath(join(system.EXEC_PATH, normpath("./../../bin")))
    system.HOME_PATH = abspath(join(system.EXEC_PATH, normpath("./../../")))
    if args.path is not None:
        system.DEST_PATH = normpath(args.path)
    else:
        system.DEST_PATH = getcwd()
    return system
