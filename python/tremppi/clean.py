import os
import sqlite3
from .header import database_file, widgets
from .configure import configure

def clean_setup(data_path, widget):
    with open(os.path.join(data_path, widget + ".js"), 'w+') as file_js:
        file_js.write('tremppi.' + widget + '.setup = {};')


def clean_data(data_path, widget):
    with open(os.path.join(data_path, widget + ".json"), 'w+') as file_json:
        file_json.write('{}')


def clean_data_files(data_path, widget):
    datafiles = [name for name in os.listdir(os.path.join(data_path, widget))]
    for file in datafiles:
        os.remove(os.path.join(data_path, widget, file))


def delete_all_files(data_path):
    for widget in widgets:
        if os.path.isdir(os.path.join(data_path, widget)):
            clean_data_files(data_path, widget)


def clean(data_path, widget):
    if widget == "editor":
        if os.path.exists(os.path.join(data_path, database_file)):
            os.remove(os.path.join(data_path, database_file))
        clean_data(data_path, "properties")
        clean_data(data_path, "select")
        delete_all_files(data_path)
    if widget == "properties":
        # set all the cost+robustness+witness data
        delete_all_files(data_path)
        clean_data(data_path, "select")
        with sqlite3.connect(os.path.join(data_path, database_file)) as conn:
            conn.execute('DROP TABLE IF EXISTS Properties')

    clean_data_files(data_path, "qualitative")
    clean_data_files(data_path, "quantitative")
    clean_data_files(data_path, "regulations")
    clean_data_files(data_path, "correlations")
    clean_data_files(data_path, "group")
    clean_data_files(data_path, "witness")

    for widget_to_conf in widgets:
        configure(data_path, widget_to_conf)