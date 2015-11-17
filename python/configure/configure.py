__author__ = 'adams_000'
import sys
import argparse
from os.path import join, dirname, abspath
if __name__ == "__main__":
    sys.path.append(dirname(dirname(abspath(sys.argv[0]))))
from tremppi.header import data_folder, widgets, system, system_init
from tremppi.configure import configure

if __name__ == "__main__":
    # define options
    parser = argparse.ArgumentParser(description='Create configurations for the HTML project browser.')
    parser.add_argument('--path', help='specify the location to update.')
    args = parser.parse_args()
    system_init(sys.argv[0], args)
    DATA_PATH = join(system.DEST_PATH, data_folder)
    # generate files
    for widget in widgets:
        configure(DATA_PATH, widget)

