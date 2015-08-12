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
    parser = argparse.ArgumentParser(description='Clean data from a TREMPPI project')
    parser.add_argument('name', help='name of the newly created model')
    args = parser.parse_args()
    system_init(sys.argv[0], args)
    shutil.rmtree(join(system.DEST_PATH, data_folder))
    generate_data(join(system.DEST_PATH, data_folder))