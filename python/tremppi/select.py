__author__ = 'adams_000'
import json
import re


def get_atom(record):
    atoms = []

    if ("select" in record) and record["select"]:
        for key, value in record.items():
            if key == 'select' or key == 'recid' or key == 'name' or key == 'changes' or value == '' or value == ' ':
                continue
            elif value == 'ANY':
                atoms.append(key + " IS NOT NULL")
            elif value == 'NULL':
                atoms.append(key + " IS NULL")
            elif re.match('!([\(\[])(\d+\.?\d*),(\d+\.?\d*)([\)\]])', value):  # interval exclusion
                matches = re.match('!([\(\[])(\d+\.?\d*),(\d+\.?\d*)([\)\]])', value)
                if matches.group(1) == '(':
                    atoms.append(key + " <= " + matches.group(2))
                else:
                    atoms.append(key + " < " + matches.group(2))
                if matches.group(4) == ')':
                    atoms.append(key + " >= " + matches.group(3))
                else:
                    atoms.append(key + " > " + matches.group(3))
            elif re.match('([\(\[])(\d+\.?\d*),(\d+\.?\d*)([\)\]])', value):  # full interval
                matches = re.match('([\(\[])(\d+\.?\d*),(\d+\.?\d*)([\)\]])', value)
                if matches.group(1) == '(':
                    atoms.append(key + " > " + matches.group(2))
                else:
                    atoms.append(key + " >= " + matches.group(2))
                if matches.group(4) == ')':
                    atoms.append(key + " < " + matches.group(3))
                else:
                    atoms.append(key + " <= " + matches.group(3))
            elif re.match('([\(\[]),(\d+\.?\d*)([\)\]])', value):  # left open interval
                matches = re.match('([\(\[]),(\d+\.?\d*)([\)\]])', value)
                if matches.group(3) == ')':
                    atoms.append(key + " < " + matches.group(2))
                else:
                    atoms.append(key + " <= " + matches.group(2))
            elif re.match('([\(\[])(\d+\.?\d*),([\)\]])', value):  # right open interval
                matches = re.match('([\(\[])(\d+\.?\d*),([\)\]])', value)
                if matches.group(1) == '(':
                    atoms.append(key + " > " + matches.group(2))
                else:
                    atoms.append(key + " >= " + matches.group(2))
            elif re.match('!\d+\.?\d*', value):
                atoms.append(key + " != " + value[1:])
            elif re.match('\d+\.?\d*', value):
                atoms.append(key + " = " + value)
            elif re.match('[+-01]', value):
                atoms.append(key + ' = "' + value + '"')
            else:
                raise Exception("The expression " + value + " for the key " + key + " is not interpreted.")
        if len(atoms) == 0:
            atoms.append("1")

    return atoms

def select_query(records):
    clauses = []

    for record in records:
        if ("select" in record) and record["select"]:
            atoms = get_atom(record)
            if len(atoms) > 0:
                clauses.append(" AND ".join(atoms))
            else:
                clauses.append("1")

    return "\n".join(clauses)

def select_name(records):
    names = []

    for record in records:
        if ("select" in record) and record["select"]:
            if ("name" in record) and record["name"] != '':
                names.append(record["name"])
            else:
                raise Exception("One of the selections does not have a name.")


    return "\n".join(names)


def select(filename, term):
    with open(filename, 'r') as selectionFile:
        grid = json.loads(selectionFile.read())
        if "records" not in grid:
            raise Exception("No selection is present.")
        if term:
            result = select_query(grid["records"])
            return result

        else:
            result = select_name(grid["records"])
            if result:
                return result
            else:
                raise Exception("No selection has been made.")
