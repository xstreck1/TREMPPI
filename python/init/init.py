import sys
import os
import argparse
import shutil
import json
from os.path import join, dirname, abspath, isfile, exists
if __name__ == "__main__":
    sys.path.append(dirname(dirname(abspath(sys.argv[0]))))
from tremppi.file_manipulation import copyanything, replace_regex, normal_paths
from tremppi.header import folders, widgets, source_folder, data_folder,configure_filename

# make sure all the data are present
def generate_data(data_path):
    if not exists(data_path):
        os.makedirs(data_path)
    for widget in widgets:
        # main json
        json_filename = join(data_path, widget + '.json')
        if not isfile(json_filename):
            with open(json_filename, 'w+') as json_file:
                json.dump({}, json_file)
        # config js
        js_filename = join(data_path, widget + '.js')
        if not isfile(js_filename):
            open(js_filename, 'w+').close()
        if not exists(join(data_path, widget)):
            os.makedirs(join(data_path, widget))

# return subdirectories that contain configure.js
def list_projects(data_path):
    result = []
    subdirs = [name for name in os.listdir(data_path) if os.path.isdir(os.path.join(data_path, name))]
    for subdir in subdirs:
        if exists(join(data_path, subdir, configure_filename)):
            result.append(subdir)
    return result

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Initiate a TREMPPI project.')
    parser.add_argument('--path', help='specify the location where the file gets created.')
    parser.add_argument('name', help='name of the newly created model')
    args = parser.parse_args()

    EXEC_PATH, BIN_PATH, HOME_PATH, DEST_PATH = normal_paths(sys.argv[0], args)
    DEST_CONTENT = join(DEST_PATH, args.name)
    if os.path.exists(DEST_CONTENT):
        raise Exception('The destination folder ' + DEST_CONTENT + ' already exists, aborting.')

    # copy the data
    for folder in folders:
        source = join(join(HOME_PATH, source_folder), folder)
        destination = join(DEST_CONTENT, folder)
        copyanything(source, destination)

    for file in widgets:
        shutil.copy(join(HOME_PATH, source_folder, file + ".html"), DEST_CONTENT)

    # make the data directory
    generate_data(join(DEST_CONTENT, data_folder))

    # create the configure data
    with open(join(DEST_CONTENT, 'configure.js'), 'w+') as setup:
        setup.write('tremppi.setup = { ' +
                    '\t"level": 0, \n' +
                    '\t"server_port": 8080, \n' +
                    '\t"server_location": "localhost", \n' +
                    '\t"project_name": "' + args.name + '"\n' +
                    '};'
                    )
