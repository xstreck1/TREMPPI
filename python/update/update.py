__author__ = 'adams_000'
import sys
import os
import argparse
import shutil
from os.path import join, dirname, abspath
if __name__ == "__main__":
    sys.path.append(dirname(dirname(abspath(sys.argv[0]))))
from tremppi.file_manipulation import copyanything, read_jsonp, write_jsonp, generate_data
from tremppi.header import folders, widgets, source_folder, system, system_init, data_folder, version, configure_filename

# define options
parser = argparse.ArgumentParser(description='Update a TREMPPI project.')
parser.add_argument('--path', help='specify the location to update.')
args = parser.parse_args()

system_init(sys.argv[0], args)
for folder in folders:
    source = join(join(system.HOME_PATH, source_folder), folder)
    destination = join(system.DEST_PATH, folder)
    if os.path.exists(destination):
        shutil.rmtree(destination)
    copyanything(source, destination)

for file in widgets:
    shutil.copy(join(system.HOME_PATH, source_folder, file + ".html"), system.DEST_PATH)
    generate_data(join(system.DEST_PATH, data_folder))

# update version
file_path = join(system.DEST_PATH, configure_filename)
header, data = read_jsonp(file_path)
data['version'] = version
write_jsonp(file_path, header, data)

shutil.copy(join(system.HOME_PATH, source_folder, "favicon.ico"), system.DEST_PATH)
shutil.copy(join(system.HOME_PATH, source_folder, "logo.png"), system.DEST_PATH)