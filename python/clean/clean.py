import sys
import os
import argparse
import shutil
from os.path import join, dirname, abspath, exists
if __name__ == "__main__":
    sys.path.append(dirname(dirname(abspath(sys.argv[0]))))
from tremppi.file_manipulation import copyanything
from tremppi.project_files import generate_data
from tremppi.header import data_folder, configure_filename, system_init, system

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Clean data from a TREMPPI project')
    parser.add_argument('--path', help='specify the location to clean.')
    args = parser.parse_args()
    system_init(sys.argv[0], args)
    if not exists(join(system.DEST_PATH, configure_filename)):
        raise Exception('The target folder ' + system.DEST_PATH + ' does not seem to be a TREMPPI project folder. The ' + configure_filename + ' is missing.')
    else:
        shutil.rmtree(join(system.DEST_PATH, data_folder))
        generate_data(join(system.DEST_PATH, data_folder))