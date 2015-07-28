import sys
import os
import argparse
import shutil
from os.path import join, dirname, abspath, exists
if __name__ == "__main__":
    sys.path.append(dirname(dirname(abspath(sys.argv[0]))))
from tremppi.file_manipulation import copyanything
from tremppi.project_files import generate_data
from tremppi.header import folders, widgets, source_folder, data_folder, configure_filename, system_init, system, version

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Initiate a TREMPPI project.')
    parser.add_argument('--path', help='specify the location where the file gets created.')
    parser.add_argument('name', help='name of the newly created model')
    args = parser.parse_args()

    system_init(sys.argv[0], args)
    DEST_CONTENT = join(system.DEST_PATH, args.name)
    if os.path.exists(DEST_CONTENT):
        raise Exception('The destination folder ' + DEST_CONTENT + ' already exists, aborting.')

    # copy the data
    for folder in folders:
        source = join(join(system.HOME_PATH, source_folder), folder)
        destination = join(DEST_CONTENT, folder)
        copyanything(source, destination)

    for file in widgets:
        shutil.copy(join(system.HOME_PATH, source_folder, file + ".html"), DEST_CONTENT)

    shutil.copy(join(system.HOME_PATH, source_folder, "logo.png"), DEST_CONTENT)
    shutil.copy(join(system.HOME_PATH, source_folder, "favicon.ico"), DEST_CONTENT)

    # make the data directory
    generate_data(join(DEST_CONTENT, data_folder))

    # create the configure data
    with open(join(DEST_CONTENT, 'configure.js'), 'w+') as setup:
        setup.write('tremppi.setup = { ' +
                    '\t"server_port": 8080, \n' +
                    '\t"server_location": "localhost", \n' +
                    '\t"project_name": "' + args.name + '",\n' +
                    '\t"version": "' + version + '"\n' +
                    '};'
                    )
