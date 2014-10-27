import sqlite3
import re
from os.path import join
from os import listdir
import json

ROW_COUNT = 10 # How many rows are fetched
SELECTION_FILE = "select/selections.js"

def get_columns_names(arguments):
    conn = sqlite3.connect('database.sqlite')
    cursor = conn.execute('select * from Parametrizations')
    names = list(map(lambda x: x[0], cursor.description))
    return "rowid," + ",".join(names)

def js_to_json(data):
    firstIndex = data.find("[")
    lastIndex = len(data) - 1
    return data[firstIndex:lastIndex]

def read_conditions(arguments):
    with open(SELECTION_FILE, 'r') as selectionFile:
        data = selectionFile.read()
        data = js_to_json(data)
        selections = json.loads(data)
        select_term = ""
        for selection in selections:
            if selection["values"][arguments["pick"][0]]:
                select_term += "(" + str(selection["values"]["Selection"]) + ") AND "
        return select_term[:-4]

def get_rows(arguments):
    conditions = read_conditions(arguments)
    if not conditions:
        return ""
    conn = sqlite3.connect('database.sqlite')
    cursor = conn.execute('SELECT rowid,* FROM Parametrizations WHERE ' + conditions)
    row_no = 0
    data = ""
    for row in cursor:
        row_no += 1
        if row_no > ROW_COUNT:
            break
        for val in row:
            data += str(val) + ","
        data = data[:-1] + '\n'
    return data[:-1]

def get_counts(arguments):
    columns = get_columns_names(arguments)
    conditions = read_conditions(arguments)
    counts = []
    for column in columns.split(","):
        if not conditions:
            counts.append("0")
        else:
            conn = sqlite3.connect('database.sqlite')
            cursor = conn.execute('SELECT COUNT(*) FROM Parametrizations WHERE ' + column + ' IS NOT NULL' + ' AND ' + conditions)
            counts.append(str(cursor.fetchone()[0]))
    return ",".join(counts)

# return current files with the .html suffix
def get_files():
    files = listdir(".")
    files = [value for value in files if re.match("^((?!browse).)*html", value)]
    print(files)
    return ",".join(files)