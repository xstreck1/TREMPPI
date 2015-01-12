import sqlite3
import re
from os.path import exists
from os import listdir
import json

DATABASE_FILE = "database.sqlite"
SELECTION_FILE = "select.json"

def get_columns_names(arguments):
    conn = sqlite3.connect(DATABASE_FILE)
    cursor = conn.execute('select * from Parametrizations')
    names = list(map(lambda x: x[0], cursor.description))
    names = [name for name in names if not name[0] == 'W']
    return "rowid," + ",".join(names)

def js_to_json(data):
    firstIndex = data.find("[")
    lastIndex = len(data) - 1
    return data[firstIndex:lastIndex]

def read_conditions(arguments):
    with open(SELECTION_FILE, 'r') as selectionFile:
        data = selectionFile.read()
        selections = json.loads(data)
        select_term = "1 AND"
        for selection in selections:
            if selection["values"][arguments["pick"][0]]:
                select_term += "(" + str(selection["values"]["Selection"]) + ") AND "
        return select_term[:-4]

def get_rows(arguments):
    conditions = read_conditions(arguments)
    columns = get_columns_names(arguments)
    if not conditions:
        return ""
    conn = sqlite3.connect(DATABASE_FILE)
    cursor = conn.execute('SELECT ' + columns + ' FROM Parametrizations WHERE ' + conditions)
    row_no = 0
    data = ""
    for row in cursor:
        row_no += 1
        if row_no > int(arguments["count"][0]):
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
        conn = sqlite3.connect(DATABASE_FILE)
        cursor = conn.execute('SELECT COUNT(*) FROM Parametrizations WHERE ' + column + ' IS NOT NULL' + ' AND ' + conditions)
        counts.append(str(cursor.fetchone()[0]))
    return ",".join(counts)

# return current files with the .html suffix
def get_files():
    if exists(DATABASE_FILE):
        files = listdir(".")
        files = [value for value in files if re.match("^((?!browse).)*html", value)]
        return ",".join(files)
    else:
        return "editor.html"