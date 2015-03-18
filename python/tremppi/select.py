__author__ = 'adams_000'
import json

def get_conditions(records):
    groups = {}
    for record in records:
        if (not "select" in record) or (not record["select"]):
            continue

        if (not ("group" in record)) or record["group"] == '':
            record["group"] = 0
        group_no = int(record["group"])

        if not (group_no in groups):
            groups[group_no] = []
        group = groups[group_no]

        group.append({})
        conditions = group[len(group)-1]

        for key, value in record.items():
            if len(key) < 4:
                continue
            elif key[-4:] == "_val" or key[-4:] == "_cmp":
                field = key[:-4]
                type = key[-3:]
                if not (field in conditions):
                    conditions[field] = (" ", 0)
                if type == "cmp":
                    conditions[field] = (value, conditions[field][1])
                elif type == "val":
                    conditions[field] = (conditions[field][0], value)
    return groups

def condition_to_string(field, cmp, val):
    result = ''

    if cmp == '' or cmp == ' ':
        result = ''
    elif cmp == '*':
        result = field
    else:
        result = field + ' ' + str(cmp) + ' ' + str(val)

    return result

def select_query(records):
    result = ''

    if len(records) == 0:
        result = "1"
    else:
        conditions = get_conditions(records)
        for index, clause in conditions.items():
            result += "("
            for conjunction in clause:
                for key, value in conjunction.items():
                    condString = condition_to_string(key, value[0], value[1])
                    if (condString != ''):
                        result += condition_to_string(key, value[0], value[1]) + ' AND '
                if (result[-5:] == ' AND '):
                    result = result[:-5]
                result += ") OR "

        if (result[-4:] == ' OR '):
            result = result[:-4]

    return result

def select(filename):
    with open(filename, 'r') as selectionFile:
        grid = json.loads(selectionFile.read())
        query = select_query(grid["records"]);
        if query != '':
            return ' WHERE ' + query
        else:
            return ""