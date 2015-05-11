# This script reads the database file and copies the necessities to the select.json file

__author__ = 'adams_000'
import sqlite3
import re
from os.path import exists
from os import listdir
import json

from tremppi.database_reader import read_components, read_regulations


def read_columns(conn):
    columns = []
    groups = []
    components = read_components(conn)
    regulations = read_regulations(conn)
    cursor = conn.execute('select * from Parametrizations')
    column_names = list(map(lambda x: x[0], cursor.description))

    columns.append({
        'field': 'select',
        'caption': '',
        'size': '25px',
        'resizable': False,
        'editable': {
            'type': 'check'
        }
    })
    columns.append({
        'field': 'name',
        'caption': 'Name',
        'size': '100px',
        'resizable': True,
        'editable': {
            'type': 'text'
        }
    })
    groups.append({
        'caption': '',
        'columns': ['select', 'name'],
        'span': 2,
        'master': False,
        'hideable': False,
        'checked': False
    })

    # Add parameters
    for comp_name in components:
        groups.append({
            'caption': 'K<sub>' + comp_name + '</sub>' + '(' + ','.join(regulations[comp_name]) + ')',
            'columns': [],
            'span': 0,
            'master': False,
            'hideable': True,
            'checked': False
        })
        for column_name in column_names:
            parts = str(column_name).split('_')
            if parts[0] == 'K' and parts[1] == comp_name:
                columns.append({
                    'field': column_name,
                    'caption': parts[2],
                    'min': 0,
                    'max': components[comp_name],
                    'size': str(12 + len(parts[2]) * 8) + 'px',
                    'type': 'text'
                })
                groups[-1]['columns'].append(column_name)
                groups[-1]['span'] += 1;

    # Add robustness
    groups.append({
        'caption': 'Robustness(property)',
        'columns': [],
        'span': 0,
        'master': False,
        'hideable': True,
        'checked': False
    })
    for column_name in column_names:
        parts = str(column_name).split('_')
        if parts[0] == 'R':
            columns.append({
                'field': column_name,
                'caption': parts[1],
                'min': 0,
                'max': 1,
                'size': str(12 + len(parts[1]) * 8) + 'px',
                'type': 'text'
            })
            groups[-1]['columns'].append(column_name)
            groups[-1]['span'] += 1

    # Add cost
    groups.append({
        'caption': 'Cost(property)',
        'columns': [],
        'span': 0,
        'master': False,
        'hideable': True,
        'checked': False
    })
    for column_name in column_names:
        parts = str(column_name).split('_')
        if parts[0] == 'R':
            columns.append({
                'field': column_name,
                'caption': parts[1],
                'min': 0,
                'max': 65536,
                'size': str(12 + len(parts[1]) * 8) + 'px',
                'type': 'text'
            })
            groups[-1]['columns'].append(column_name)
            groups[-1]['span'] += 1

    return columns, groups


def tabularize(database, target_file):
    with sqlite3.connect(database) as conn:
        with open(target_file, 'w+') as select_js:
            columns, groups = read_columns(conn)
            grid = {'columns': columns, 'groups': groups}
            select_js.write("tremppi.select.grid_structure = ")
            json.dump(grid, select_js)
            select_js.write(";")
