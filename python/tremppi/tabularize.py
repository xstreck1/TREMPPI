# This script reads the database file and copies the necessities to the select.json file

__author__ = 'adams_000'
import sqlite3
import re
from os.path import exists
from os import listdir
import json

from tremppi.database_reader import read_components, read_regulations

def read_columns(conn):
    result = []
    components = read_components(conn)
    cursor = conn.execute('select * from Parametrizations')
    names = list(map(lambda x: x[0], cursor.description))
    for name in names:
        parts = str(name).split("_")
        if (parts[0] == "K"):
            result.append({
                "field": name
            })
        elif (parts[0] == "R"):
            result.append({
                "field": name
            })
        elif (parts[0] == "C"):
            result.append({
                "field": name
            })
    return result

def tabularize(database, target_file):
    with sqlite3.connect(database) as conn:
        grid = {}
        with open(target_file, 'r') as select_json:
            grid = json.loads(select_json.read())

        grid["regulations"] = read_regulations(conn)
        grid["columns"] = read_columns(conn)
        with open(target_file, 'w') as select_json:
            json.dump(grid, select_json)
