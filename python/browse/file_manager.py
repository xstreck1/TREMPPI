__author__ = 'adams_000'


def save_file(store_path, data):
    with open(store_path, 'w') as file:
        file.write(data.decode())

def get_log(log_path):
    with open(log_path, 'r') as file:
        return file.read()