__author__ = 'adams_000'


def save_file(store_path, data):
    with open(store_path, 'w') as fh:
        fh.write(data.decode())