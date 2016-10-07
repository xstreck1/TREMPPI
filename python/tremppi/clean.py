import os
from tremppi.header import database_file, widgets


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
        os.remove(os.path.join(data_path, database_file))
        clean_data("properties")
        clean_data("select")
        delete_all_files(data_path)
    if widget == "properties":
        # set all the cost+robustness+witness data
        delete_all_files(data_path)
