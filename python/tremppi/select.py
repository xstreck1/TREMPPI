__author__ = 'adams_000'
import json
import re


def get_atom(record):
    atoms = []
    if (not "select" in record) or (not record["select"]):
        return

    for key, value in record.items():
        if key == 'select' or key == 'recid':
            continue
        elif re.match('\d+.*\d*', value):
            atoms.append(key + " = " + value)
        elif re.match('([\(\[])(\d+.*\d*),(\d+.*\d*)([\)\]])', value):
            matches = re.match('([\(\[])(\d+.*\d*),(\d+.*\d*)([\)\]])', value)
            if matches.group(1) == '(':
                atoms.append(key + " > " + matches.group(2))
            else:
                atoms.append(key + " >= " + matches.group(2))
            if matches.group(4) == ')':
                atoms.append(key + " < " + matches.group(3))
            else:
                atoms.append(key + " <= " + matches.group(3))
        else:
            raise "The expression " + value + " for the key " + key + " is not interpreted."

    if len(atoms) == 0:
        atoms.push("1")
    return atoms

def select_query(records):
    clauses = []

    for record in records:
        clauses.append("(" + " AND ".join(get_atom(record)) + ")")

    return " OR ".join(clauses)

def select(filename):
    with open(filename, 'r') as selectionFile:
        grid = json.loads(selectionFile.read())
        if "records" not in grid:
            return ""
        query = select_query(grid["records"])
        if query != '':
            return ' WHERE ' + query
        else:
            raise "No selection has been made."