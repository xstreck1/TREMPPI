from os import replace, remove, fdopen, chdir, listdir
from os.path import dirname, join, basename, exists, abspath, split
from subprocess import Popen

from init.init import init
from tremppi.header import data_folder, default_port, configure_filename, projects_filename, last_page_filename, system_init, system,  database_file
from tremppi.project_files import list_projects, write_projects, generate_data,  delete_project, save_file, get_log, get_path_level
from tremppi.file_manipulation import copyanything, read_jsonp, write_jsonp
from tremppi.configure import configure


def mk_usr_proj(target_folder):
    # check for the last selection by the user
    last_page = ""
    if exists(join(target_folder, last_page_filename)):
        with open(join(target_folder, last_page_filename), "r") as last_page_file:
            last_page = last_page_file.read()

    # the case the folder is empty
    if not listdir(target_folder):
        process = Popen([join(system.BIN_PATH, "tremppi")] + ['init'] + ['project_0'] + ['--path'] + [abspath(target_folder)])
        process.wait()
        write_projects(target_folder)
    # the case that we are in a single project
    if exists(join(target_folder, configure_filename)):
        generate_data(join(target_folder, data_folder))
        # select the opening page
        if last_page is not "" and exists(join(target_folder,last_page)):
            project_path = last_page
        else:
            project_path = "index.html"
    # the case we have multiple projects (or the folder was empty)
    elif exists(join(target_folder, projects_filename)):
        # get all projects, create a new one if empty
        projects = list_projects(target_folder)
        if not projects:
            raise Exception('Projects missing from the project folder. Empty the folder and restart TREMPPI.')

        # select the opening page
        if last_page is not "" and exists(join(target_folder,last_page)):
            project_path = last_page
        else:
            project_path = projects[0] + "/index.html"

        # generate data on all the projects
        for project_folder in projects:
            generate_data(join(target_folder, project_folder, data_folder))
        write_projects(target_folder)
    else:
        raise Exception(configure_filename + " or " + projects_filename + " does not exist in " + target_folder + ", which is not empty. Are you sure it's a correct path?")

    return project_path
