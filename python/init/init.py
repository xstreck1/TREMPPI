import sys
import os
import argparse
import shutil
from os.path import join, dirname, abspath
sys.path.append(dirname(dirname(abspath(sys.argv[0]))))
from tremppi.file_manipulation import copyanything, replace_regex, normal_paths
from tremppi.header import folders, files, source_folder

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

for file in files:
    html_file = join(source_folder, file + '.html')
    shutil.copy(join(HOME_PATH, html_file, DEST_CONTENT))

replace_regex(join(DEST_PATH, 'common', 'tremppi.js'), 'project_name:.*,', 'project_name: ' + args.name + ',')