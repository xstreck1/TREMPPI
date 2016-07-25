# Created by Adam Streck, 2013-2015, adam.streck@fu-berlin.de
#
# This file is part of the Toolkit for Reverse Engineering of Molecular Pathways
# via Parameter Identification (TREMPPI)
#
# This program is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with
# this program.  If not, see <http://www.gnu.org/licenses/>.

import json
import os.path
import re
import sqlite3
from .database_reader import component_regulators_list, read_components, read_regulations
from .header import widgets, database_file

def add_basics(columns, groups):
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


def get_val_interval(source, current_thr, target, components, regulations):
    thresholds = []
    for comp_name, comp_max in components:
        if comp_name == source:
            thresholds.append(comp_max + 1)
            break
    for r_source, r_threshold, r_target in regulations:
        if r_source == source and r_target == target:
            thresholds.append(int(r_threshold))
    thresholds.sort()
    next_threshold = 0
    for thr in thresholds:
        if thr > current_thr:
            next_threshold = thr
            break
    return '{' + ','.join(map(str, range(current_thr, next_threshold))) + '}'


def add_parameters(conn, components, columns, groups, column_names):
    component_regs_list = component_regulators_list(conn)
    regulations = read_regulations(conn)
    for comp_name, comp_max in components:
        reg_tuple = ""
        if comp_name in component_regs_list:
            reg_tuple = ','.join(component_regs_list[comp_name])

        groups.append({
            'caption': 'K<sub>' + comp_name + '</sub>(' + reg_tuple + ')',
            'columns': [],
            'span': 0,
            'master': False,
            'hideable': True
        })
        for column_name in column_names:
            parts = str(column_name).split('_')
            if re.match('K_' + comp_name + '_(.*)', column_name):
                digits = re.sub('K_' + comp_name + '_(.*)', '\\1', column_name)
                ranges = []
                for i in range(len(digits)):
                    source = component_regs_list[comp_name][i]
                    ranges.append(get_val_interval(source, int(digits[i]), comp_name, components, regulations))
                caption = ','.join(ranges)
                columns.append({
                    'field': column_name,
                    'caption': caption,
                    'size': str(len(caption) * 8 + 6) + 'px',
                    'editable': {
                        'min': 0,
                        'max': comp_max,
                        'type': 'text'
                    },
                    "resizable": True
                })
                groups[-1]['columns'].append(column_name)
                groups[-1]['span'] += 1


def add_sign(columns, groups, column_names):
    new_group = {
        'caption': 'Sign(edge)',
        'columns': [],
        'span': 0,
        'master': False,
        'hideable': True
    }
    for column_name in column_names:
        parts = str(column_name).split('_')
        if re.match('S_(.*)', column_name):
            columns.append({
                'field': column_name,
                'caption': re.sub('S_(.*)_(\d)_(.*)', '\\1,\\2,\\3', column_name),
                'size': str(len(column_name) * 8) + 'px',
                'editable': {
                    'type': 'select',
                    'items': ["1", "0", "+", "-", " "]
                },
                "resizable": True
            })
            new_group['columns'].append(column_name)
            new_group['span'] += 1
    if new_group['span'] > 0:
        groups.append(new_group)


def add_indegree(columns, groups, column_names):
    new_group = {
        'caption': 'Indegree(component)',
        'columns': [],
        'span': 0,
        'master': False,
        'hideable': True
    }
    for column_name in column_names:
        parts = str(column_name).split('_')
        if re.match('E_(.*)', column_name):
            columns.append({
                'field': column_name,
                'caption': re.sub('E_(.*)', '\\1', column_name),
                'size': str(len(column_name) * 8) + 'px',
                'editable': {
                    'min': 0,
                    'max': 1024,
                    'type': 'text'
                },
                "resizable": True
            })
            new_group['columns'].append(column_name)
            new_group['span'] += 1
    if new_group['span'] > 0:
        groups.append(new_group)


def add_bias(columns, groups, column_names):
    new_group = {
        'caption': 'Bias(component)',
        'columns': [],
        'span': 0,
        'master': False,
        'hideable': True
    }
    for column_name in column_names:
        parts = str(column_name).split('_')
        if re.match('B_(.*)', column_name):
            columns.append({
                'field': column_name,
                'caption': re.sub('B_(.*)', '\\1', column_name),
                'size': str(len(column_name) * 8) + 'px',
                'editable': {
                    'min': 0,
                    'max': 1,
                    'type': 'text'
                },
                "resizable": True
            })
            new_group['columns'].append(column_name)
            new_group['span'] += 1
    if new_group['span'] > 0:
        groups.append(new_group)


def add_impact(columns, groups, column_names):
    new_group = {
        'caption': 'Impact(edge)',
        'columns': [],
        'span': 0,
        'master': False,
        'hideable': True
    }
    for column_name in column_names:
        parts = str(column_name).split('_')
        if re.match('I_(.*)', column_name):
            columns.append({
                'field': column_name,
                'caption': re.sub('I_(.*)_(\d)_(.*)', '\\1,\\2,\\3', column_name),
                'size': str(len(column_name) * 8) + 'px',
                'editable': {
                    'min': 0,
                    'max': 1,
                    'type': 'text'
                },
                "resizable": True
            })
            new_group['columns'].append(column_name)
            new_group['span'] += 1
    if new_group['span'] > 0:
        groups.append(new_group)


def add_cost(columns, groups, column_names):
    new_group = {
        'caption': 'Cost(property)',
        'columns': [],
        'span': 0,
        'master': False,
        'hideable': True
    }
    for column_name in column_names:
        if re.match('C_(.*)', column_name):
            columns.append({
                'field': column_name,
                'caption': re.sub('C_(.*)', '\\1', column_name),
                'size': str(len(column_name) * 8) + 'px',
                'editable': {
                    'min': 0,
                    'max': 1024,
                    'type': 'text'
                },
                "resizable": True
            })
            new_group['columns'].append(column_name)
            new_group['span'] += 1
    if new_group['span'] > 0:
        groups.append(new_group)


def add_robustness(columns, groups, column_names):
    new_group = {
        'caption': 'Robustness(property)',
        'columns': [],
        'span': 0,
        'master': False,
        'hideable': True
    }
    for column_name in column_names:
        if re.match('R_(.*)', column_name):
            columns.append({
                'field': column_name,
                'caption': re.sub('R_(.*)', '\\1', column_name),
                'size': str(len(column_name) * 8) + 'px',
                'editable': {
                    'min': 0,
                    'max': 1,
                    'type': 'text'
                },
                "resizable": True
            })
            new_group['columns'].append(column_name)
            new_group['span'] += 1
    if new_group['span'] > 0:
        groups.append(new_group)


def make_selection(conn):
    columns = []
    groups = []
    components = read_components(conn)
    cursor = conn.execute('select * from Parametrizations')
    column_names = list(map(lambda x: x[0], cursor.description))
    add_basics(columns, groups)
    add_parameters(conn, components, columns, groups, column_names)
    add_sign(columns, groups, column_names)
    add_indegree(columns, groups, column_names)
    add_bias(columns, groups, column_names)
    add_impact(columns, groups, column_names)
    add_cost(columns, groups, column_names)
    add_robustness(columns, groups, column_names)
    return columns, groups


def make_group(conn):
    columns = [{
        'field': 'count',
        'caption': 'Count',
        'size': '50px',
        "resizable": True
    }]
    groups = []
    components = read_components(conn)
    cursor = conn.execute('select * from Parametrizations')
    column_names = list(map(lambda x: x[0], cursor.description))
    add_sign(columns, groups, column_names)
    add_cost(columns, groups, column_names)
    return columns


def make_group_menu(conn):
    menu_items = []
    components = read_components(conn)
    cursor = conn.execute('select * from Parametrizations')
    column_names = list(map(lambda x: x[0], cursor.description))

    for column_name in column_names:
        if re.match('C_(.*)', column_name):
            menu_items.append({
                'id' : column_name,
                'text' : re.sub('C_(.*)', '\\1', column_name),
                'checked': True
            })
        if re.match('S_(.*)', column_name):
            menu_items.append({
                'id' : column_name,
                'text' : re.sub('S_(.*)_(\d)_(.*)', '\\1,\\2,\\3', column_name),
                'checked': True
            })
    return menu_items


def make_list(conn):
    columns = [
        {
            'field': 'select', 'caption': '', 'size': '25px', 'resizable': False, 'editable': {'type': 'check'}
        },
        {
            'field': 'name', 'caption': 'Name', 'size': '200px', 'resizable': True, 'sortable': True,
            'editable': {'type': 'text'}
        },
        {
            'field': 'ending', 'caption': 'Ending', 'size': '70px', 'resizable': True, 'editable':
            {
                'type': 'select', 'items': ["open", "stable", "cyclic"]
            }
        },
        # {
        #    'field': 'bound', 'caption': 'Bound', 'size': '60px', 'resizable': True, 'editable': {'type': 'text'}
        # }
    ]
    components = read_components(conn)
    for comp_name, comp_max in components:
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
    columns = [{'field': 'id', 'caption': 'Id', 'size': '30px'}]
    groups = [{'span': 1, 'caption': ''}]
    components = read_components(conn)
    for comp_name, comp_max in components:
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
        with sqlite3.connect(os.path.join(data_path, database_file)) as conn:
            with open(os.path.join(data_path, "select.js"), 'w+') as select_js:
                columns, groups = make_selection(conn)
                configured = {'columns': columns, 'groups': groups}
                select_js.write("tremppi.select.setup = ")
                json.dump(configured, select_js)
                select_js.write(";")
    elif widget == "properties":
        with sqlite3.connect(os.path.join(data_path, database_file)) as conn:
            with open(os.path.join(data_path, "properties.js"), 'w+') as properties_js:
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
    elif widget in ["qualitative", "quantitative", "regulations", "correlations", "witness", "group"]:  # generate files list
        files = []
        widget_dir = os.path.join(data_path, widget)
        if not os.path.exists(widget_dir):
            os.makedirs(widget_dir)
        else:
            for file in os.listdir(widget_dir):
                if file.endswith(".json"):
                    files.append(file[:-5])

        with sqlite3.connect(os.path.join(data_path, database_file)) as conn:
            with open(os.path.join(data_path, widget + '.js'), 'w+') as file_js:
                file_js.write('tremppi.' + widget + '.setup = ')
                json_data = {"files": files, "components": [comp[0] for comp in read_components(conn)]}
                if widget == "group":
                    json_data['columns'] = make_group(conn)
                    json_data['menu_items'] = make_group_menu(conn)
                json.dump(json_data, file_js)
                file_js.write(';')

    elif widget in widgets:
        js_filename = os.path.join(data_path, widget + '.js')
        open(js_filename, 'w+').close()
    else:
        raise Exception("Unknown widget " + widget)
