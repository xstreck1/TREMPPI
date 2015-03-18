# This script reads the database file and copies the necessities to the select.json file

__author__ = 'adams_000'
import sqlite3
import re
from os.path import exists
from os import listdir
import json

PARAMETRIZATIONS_TABLE = "Parametrizations"

def read_components(conn):
    result = {}
    cursor = conn.execute('SELECT Name, MaxActivity FROM Components')
    for row in cursor:
        result[str(row[0])] = int(row[1])
    return result

def read_regulations(conn):
    result = {}
    cursor = conn.execute('SELECT DISTINCT Target, Source FROM Regulations ORDER BY Target')
    for row in cursor:
        if str(row[0]) not in result:
            result[str(row[0])] = []
        result[str(row[0])].append(str(row[1]))
    return result

def read_columns(conn):
    result = []
    cursor = conn.execute('select * from Parametrizations')
    names = list(map(lambda x: x[0], cursor.description))
    for name in names:
        result.append({
            "editable": {
                "max": 9,
                "min": 0,
                "type": "int"
            },
            "field": name
        })

    return result

def tabulize(database, target_file):
    with sqlite3.connect(database) as conn:
        grid = {}
        with open(target_file, 'r') as select_json:
            grid = json.loads(select_json.read())

        grid["regulations"] = read_regulations(conn)
        grid["columns"] = read_columns(conn)

        with open(target_file, 'w') as select_json:
            json.dump(grid, select_json)
