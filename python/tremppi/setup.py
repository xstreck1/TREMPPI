__author__ = 'adams_000'
import sys
import os
import argparse
import shutil
from os.path import join, dirname, abspath
sys.path.append(dirname(dirname(abspath(sys.argv[0]))))
from tremppi.file_manipulation import copyanything, replace, normal_paths
from tremppi.header import folders, files, source_folder