__author__ = 'adams_000'
import sys
import argparse
import os
import json
import sqlite3
from os.path import join, dirname, abspath, exists

sys.path.append(dirname(dirname(abspath(sys.argv[0]))))
from tremppi.database_reader import read_regulations, read_components
from tremppi.header import data_folder, widgets, database_file
from tremppi.file_manipulation import normal_paths


def make_selection(conn):
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
        'hideable': False
    })

    # Add parameters
    for comp_name in components:
        groups.append({
            'caption': 'K<sub>' + comp_name + '</sub>' + '(' + ','.join(regulations[comp_name]) + ')',
            'columns': [],
            'span': 0,
            'master': False,
            'hideable': True
        })
        for column_name in column_names:
            parts = str(column_name).split('_')
            if parts[0] == 'K' and parts[1] == comp_name:
                columns.append({
                    'field': column_name,
                    'caption': parts[2],
                    'size': str(12 + len(parts[2]) * 8) + 'px',
                    'editable': {
                        'min': 0,
                        'max': components[comp_name],
                        'type': 'text'
                    },
                    "resizable": True
                })
                groups[-1]['columns'].append(column_name)
                groups[-1]['span'] += 1

    # Add robustness
    groups.append({
        'caption': 'Robustness(property)',
        'columns': [],
        'span': 0,
        'master': False,
        'hideable': True
    })
    for column_name in column_names:
        parts = str(column_name).split('_')
        if parts[0] == 'R':
            columns.append({
                'field': column_name,
                'caption': parts[1],
                'size': str(12 + len(parts[1]) * 8) + 'px',
                'editable': {
                    'min': 0,
                    'max': 1,
                    'type': 'text'
                },
                "resizable": True
            })
            groups[-1]['columns'].append(column_name)
            groups[-1]['span'] += 1

    # Add cost
    groups.append({
        'caption': 'Cost(property)',
        'columns': [],
        'span': 0,
        'master': False,
        'hideable': True
    })
    for column_name in column_names:
        parts = str(column_name).split('_')
        if parts[0] == 'R':
            columns.append({
                'field': column_name,
                'caption': parts[1],
                'size': str(12 + len(parts[1]) * 8) + 'px',
                'editable': {
                    'min': 0,
                    'max': 65536,
                    'type': 'text'
                },
                "resizable": True
            })
            groups[-1]['columns'].append(column_name)
            groups[-1]['span'] += 1

    return columns, groups


def make_list(conn):
    columns = [
        {'field': 'name', 'caption': 'Name', 'size': '200px', 'resizable': True, 'sortable': True,
         'editable': {'type': 'text'}
         },
        {'field': 'ending', 'caption': 'Ending', 'size': '70px', 'resizable': True,
         'editable': {
             'type': 'select',
             'items': [
                 "any",
                 "stable",
                 "goto A",
                 "goto B",
                 "goto C",
                 "goto D",
                 "goto E",
                 "goto F",
                 "goto G",
                 "goto H",
                 "goto I",
                 "goto J",
                 "goto K",
                 "goto L",
                 "goto M",
                 "goto N",
                 "goto O",
                 "goto P",
                 "goto Q",
                 "goto R",
                 "goto S",
                 "goto T",
                 "goto U",
                 "goto V",
                 "goto W",
                 "goto X",
                 "goto Y",
                 "goto Z"
                 ]
            }
         },
        {'field': 'validate', 'caption': 'V', 'size': '20px', 'resizable': False,
         'editable': {'type': 'checkbox'}
         },
        {'field': 'witness', 'caption': 'W', 'size': '20px', 'resizable': False,
         'editable': {'type': 'checkbox'}
         },
        {'field': 'robustness', 'caption': 'R', 'size': '20px', 'resizable': False,
         'editable': {'type': 'checkbox'}
         },
        {'field': 'simulate', 'caption': 'S', 'size': '20px', 'resizable': False,
         'editable': {'type': 'checkbox'}
         },
        {'field': 'bound', 'caption': 'Bound', 'size': '60px', 'resizable': True,
         'editable': {'type': 'text'}
         }
    ]
    components = read_components(conn)
    for comp_name in components:
        columns.append({
            'field': 'E_' + comp_name,
            'caption': comp_name,
            'size': '46px',
            'editable': {
                'type': 'text'
            }
        })
    return columns


def make_detail(conn):
    columns = []
    groups = []
    components = read_components(conn)
    for comp_name in components:
        groups.append({
            'span': 2,
            'caption': comp_name
        })
        columns.append({
            'field': 'V_' + comp_name,
            'caption': 'Value',
            'size': '46px',
            'editable': {
                'type': 'text'
            }
        })
        columns.append({
            'field': 'D_' + comp_name,
            'caption': 'Delta',
            'size': '44px',
            'editable': {
                'type': 'select',
                'items': ["", "up", "down", "stay"]
            }
        })
    return columns, groups


# Creates the configuration files
def configure(data_path, widget):
    if widget == "select":
        with sqlite3.connect(join(data_path, database_file)) as conn:
            with open(join(data_path, "select.js"), 'w+') as select_js:
                columns, groups = make_selection(conn)
                configured = {'columns': columns, 'groups': groups}
                select_js.write("tremppi.select.setup = ")
                json.dump(configured, select_js)
                select_js.write(";")
    elif widget == "properties":
        with sqlite3.connect(join(data_path, database_file)) as conn:
            with open(join(data_path, "properties.js"), 'w+') as properties_js:
                list_columns = make_list(conn)
                detail_columns, detail_groups = make_detail(conn)
                configured = {
                    'list_columns': list_columns,
                    'detail_columns': detail_columns,
                    'detail_groups': detail_groups
                }
                properties_js.write("tremppi.properties.setup = ")
                json.dump(configured, properties_js)
                properties_js.write(";")
    elif widget in ["qualitative", "quantitative", "interact", "function"]:
        files = []
        widget_dir = join(data_path, widget)
        if not exists(widget_dir):
            os.makedirs(widget_dir)
        else:
            for file in os.listdir(widget_dir):
                if file.endswith(".json"):
                    files.append(file)
        with open(join(data_path, widget + '.js'), 'w+') as file_js:
            file_js.write('tremppi.' + widget + '.setup = ')
            json.dump(files, file_js)
            file_js.write(';')
    elif widget in widgets:
        js_filename = join(DATA_PATH, widget + '.js')
        open(js_filename, 'w+').close()
    else:
        raise "Unknown widget " + widget


if __name__ == "__main__":
    # define options
    parser = argparse.ArgumentParser(description='Create configurations for the HTML project browser.')
    parser.add_argument('--path', help='specify the location to update.')
    args = parser.parse_args()
    EXEC_PATH, BIN_PATH, HOME_PATH, DEST_PATH = normal_paths(sys.argv[0], args)
    DATA_PATH = join(DEST_PATH, data_folder)
    # generate files
    for widget in widgets:
        configure(DATA_PATH, widget)
