# This script reads the database file and copies the necessities to the select.json file

__author__ = 'adams_000'
import sqlite3
import re
from os.path import exists
from os import listdir
import json

COMPONENTS_TABLE = "Components"
REGULATIONS_TABLE = "Regulations"
PARAMETRIZATIONS_TABLE = "Parametrizations"

def read_components(conn):
    cursor = conn.execute('select * from ' + COMPONENTS_TABLE)

def tabulize(database, target_file):
    with sqlite3.connect(database) as conn:

        with open(target_file, 'r') as select_json:
            grid = json.loads(select_json.read())
