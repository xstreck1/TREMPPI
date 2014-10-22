import sqlite3
import re
from os.path import join
from os import listdir

ROW_COUNT = 10 # How many rows are fetched

def get_columns_names():
    conn = sqlite3.connect('database.sqlite')
    cursor = conn.execute('select * from Parametrizations')
    names = list(map(lambda x: x[0], cursor.description))
    return "\n".join(names)

def get_rows():
    conn = sqlite3.connect('database.sqlite')
    cursor = conn.execute('select * from Parametrizations')
    row_no = 0
    data = ""
    for row in cursor:
        row_no += 1
        if row_no > ROW_COUNT:
            break
        for val in row:
            data += str(val) + " "
        data += "\n"
    return data

# return current files with the .html suffix
def get_files():
    files = listdir(".")
    files = [value for value in files if re.match("^((?!browse).)*html", value)]
    print(files)
    return "\n".join(files)