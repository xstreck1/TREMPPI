import sys
import os
import argparse
import shutil
from os.path import join, dirname, abspath

sys.path.append(dirname(dirname(abspath(sys.argv[0]))))
from tremppi.file_manipulation import copyanything, replace_regex, normal_paths
from tremppi.header import folders, widgets, source_folder

parser = argparse.ArgumentParser(description='Initiate a TREMPPI project.')
parser.add_argument('--path', help='specify the location where the file gets created.')
parser.add_argument('name', help='name of the newly created model')
args = parser.parse_args()

EXEC_PATH, BIN_PATH, HOME_PATH, DEST_PATH = normal_paths(sys.argv[0], args)

DEST_CONTENT = join(DEST_PATH, args.name)
if os.path.exists(DEST_CONTENT):
    raise 'The destination folder ' + DEST_CONTENT + ' already exists, aborting.'

# copy the data
for folder in folders:
    source = join(join(HOME_PATH, source_folder), folder)
    destination = join(DEST_CONTENT, folder)
    copyanything(source, destination)

# make the data directory
os.makedirs(join(DEST_CONTENT, 'data'))

for widget in widgets:
    html_file = join(source_folder, widget + '.html')
    shutil.copy(join(HOME_PATH, html_file, DEST_CONTENT))
    open(join(DEST_CONTENT, 'data', widget +'.js')) #create an empty data configure file

# create the configure data
with open(join(DEST_CONTENT, 'configure.js'), 'w+') as setup:
    setup.write('tremppi.configure = { ' +
                'server_port: 8080' +
                'server_location: "localhost"' +
                'project_name: ' + args.name)
