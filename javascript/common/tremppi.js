/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global w2ui */

tremppi = {
    widgets: ['index', 'editor', 'select', 'properties', 'quantitative', 'qualitative', 'regulations', 'correlations', 'witness'],
    widgetInterface: function () {
        return {
            page: function () {
                tremppi.log("page not implemented", "warning");
            },
            layout: function () {
                tremppi.log("layout not implemented", "warning");
            },
            setData: function (data) {
                tremppi.log("setData not implemented", "warning");
            },
            controls: function () {
                tremppi.log("controls not implemented", "warning");
            },
            toolbarClass: function () {
                tremppi.log("toolbarClass not implemented", "warning");
                return {};
            }
        };
    },
    getRootAddress: function () {
        return "http://" + tremppi.setup.server_location + ":" + tremppi.setup.server_port + "/";
    },
    getProjectAddress: function () {
        return tremppi.getRootAddress() + tremppi.project_folder;
    },
    makeDataFilepath: function (filename) {
        if (typeof filename === 'undefined') {
            return "./data/" + tremppi.widget_name + ".json";
        } else {
            return "./data/" + tremppi.widget_name + "/" + filename + ".json";
        }
    },
    saveData: function (data, filename) {
        if (typeof data === 'undefined') {
            tremppi.log("no data specified in saveData", "error");
        }
        var content = JSON.stringify(data, null, '\t');
        var url = tremppi.getProjectAddress() + tremppi.makeDataFilepath(filename) + "?save";
        $.ajax({
            type: "POST",
            url: url,
            data: content,
            success: function (res) {
                tremppi.log(url + " saved successfully.");
            },
            error: function (res) {
                tremppi.log("Save of " + url + " failed! Data were not saved!", 'error');
            }
        });
    },
    getData: function (callback, filename) {
        var to_load = tremppi.makeDataFilepath(filename);
        $.getJSON(to_load, callback)
                .done(function () {
                    tremppi.log(to_load + " loaded successfully.");
                })
                .fail(function (jqXHR, textStatus, errorThrown) {
                    tremppi.log("Load of the file " + to_load + " failed!", 'error');
                });
    },
    makeStorageKey: function (key) {
        return "tremppi." + tremppi.project_name + "." + tremppi.widget_name + "." + key;
    },
    setItem: function (key, value) {
        if (typeof localStorage === 'undefined') {
            tremppi.log("localStorage not available, browser key " + key + " not stored", "warning");
        }
        else {
            localStorage.setItem(tremppi.makeStorageKey(key), value);
        }
    },
    getItem: function (key, default_value) {
        if (typeof localStorage === 'undefined') {
            tremppi.log("localStorage not available, browser key " + key + " not loaded", "warning");
            return null;
        }
        else {
            var value = localStorage.getItem(tremppi.makeStorageKey(key));
            if (value === null) {
                if (typeof default_value === 'undefined') {
                    return null;
                }
                else {
                    return default_value;
                }
            }
            else {
                return value;
            }
        }
    },
    setPage: function () {
        var url = tremppi.getProjectAddress() + tremppi.widget_name + ".html?page";
        $.ajax({
            type: "POST",
            url: url
        });
    },
    makeScript: function (src) {
        var element = document.createElement('script');
        element.src = src;
        return element;
    },
    makeLink: function (rel, type, href) {
        var element = document.createElement('link');
        element.rel = rel;
        element.type = type;
        element.href = href;
        return element;
    },
    makeHead: function () {
        var head = document.getElementsByTagName('head')[0];

        // common 
        head.appendChild(tremppi.makeLink('icon', 'image/x-icon', './favicon.ico'));
        head.appendChild(tremppi.makeScript('./configure.js?_=' + Math.random().toString().slice(2))); // load the setup with cache busting
        head.appendChild(tremppi.makeScript('../projects.js?_=' + Math.random().toString().slice(2))); // load the projects list with cache busting
        head.appendChild(tremppi.makeLink('stylesheet', 'text/css', './common/css.css'));
        head.appendChild(tremppi.makeScript('./common/common.js'));

        // widget-related
        head.appendChild(tremppi.makeLink('stylesheet', 'text/css', './' + tremppi.widget_name + '/css.css'));
        head.appendChild(tremppi.makeScript('./' + tremppi.widget_name + '/widget.js'));
        head.appendChild(tremppi.makeScript('./' + tremppi.widget_name + '/controls.js'));
        head.appendChild(tremppi.makeScript('./' + tremppi.widget_name + '/page.js'));
        head.appendChild(tremppi.makeScript('./data/' + tremppi.widget_name + '.js?_=' + Math.random().toString().slice(2)));

        document.title = tremppi.widget_name;
    },
    configure: function () {
        if (typeof tremppi.projects === 'undefined') {
            tremppi.project_name = tremppi.setup.project_name;
            tremppi.projects = [tremppi.setup.project_name];
            tremppi.project_folder = "";
            tremppi.level = 0;
        }
        else {
            tremppi.project_name = window.location.pathname.split("/")[1];
            tremppi.project_folder = tremppi.project_name + "/";
            tremppi.level = 1;
        }
        tremppi.final = typeof tremppi.setup.final === 'undefined' ? false : tremppi.setup.final;
        tremppi.current_file = '/data/' + tremppi.widget_name + '.json';
        tremppi.current_object = tremppi.widget_name;
    },
    makeBody: function () {
        tremppi.configure();
        tremppi.setPage();

        // Set basic layout
        var layout_style = 'border: 0px solid #dfdfdf;';
        var layout = {
            name: 'layout',
            panels: [
                {type: 'left', style: layout_style, size: 200, content: '<div id="files" ></div>'},
                {type: 'main', style: layout_style, content: '<div id="widget" ></div>', toolbar: tremppi.widget.toolbarClass()},
                {type: 'bottom', size: 20, content: '<div id="log_line" ></div>'}
            ]
        };

        tremppi.layout = $('body').w2layout(layout);
        tremppi.toolbar = w2ui.layout.get("main").toolbar;
        // Distable toolbar for final projects
        if (tremppi.final || tremppi.toolbar.items.length === 0) {
            w2ui.layout.hideToolbar('main');
        }

        var select_name = '"' + (tremppi.widget_name === 'index' ? tremppi.project_name : '') + '"';
        // Set left side bar
        if (tremppi.final) {
            var project_controls = '<div id="static_text">FINALIZED PROJECT</div>';
        }
        else {
            if (tremppi.widget_name === 'tools') {
                if (tremppi.level === 1) {
                    var project_controls =
                            '<button class="btn" id="new_project_btn" onclick="tremppi.newProject()" >NEW PROJECT</button>' +
                            '<button class="btn" id="clone_btn" onclick="tremppi.cloneProject()" >CLONE</button>' +
                            '<input  id="select_name" type="text" name="Fill to create or rename" value=' + select_name + '>' +
                            '<button class="btn" id="rename_btn" onclick="tremppi.renameProject()" >RENAME</button>' +
                            '<button class="btn" id="delete_btn" onclick="tremppi.deleteProject()" >DELETE</button>' +
                            '<button class="btn" id="finalize_btn" onclick="tremppi.finalizeProject()" >FINALIZE</button>';
                }
                else {
                    var project_controls =
                            '<button class="btn" id="finalize_btn" onclick="tremppi.finalizeProject()" >FINALIZE</button>';
                }
            }
            else if (["index", "editor", "properties", "select"].indexOf(tremppi.widget_name) !== -1) {
                var project_controls = '<button id="save_btn" onclick="tremppi.save()" class="btn">SAVE</button>';
            }
            else if (["qualitative", "quantitative", "regulations", "correlations", "witness"].indexOf(tremppi.widget_name) !== -1) {
                var project_controls =
                        '<input id="select_name" type="text" name="Fill to create or rename" value=' + select_name + '>'
                        + '<button id="rename_btn" onclick="tremppi.rename()" class="btn">RENAME</button>'
                        + '<button id="delete_btn" onclick="tremppi.delete()" class="btn">DELETE</button>'
                        // + '<button id="save_btn" onclick="tremppi.save()" class="btn">SAVE</button>'
                        ;
            }
        }

        var sidebar = {
            name: 'sidebar',
            nodes: [],
            topHTML:
                    '<div class="sidebar_field" id="sidebar_field" style="height: 120px;" >' +
                    '<img id="logo" src="logo.png" />' +
                    '</div>',
            bottomHTML:
                    '<div class="sidebar_field">' +
                    project_controls +
                    '</div>'
        };


        // Add the projects
        if (tremppi.level === 1) {
            sidebar.nodes.push({
                id: 'projects', text: 'projects', expanded: true, group: true,
                nodes: tremppi.projects.map(function (name) {
                    return {id: 'project+' + name, text: name, selected: (name === tremppi.project_name)};
                })
            });
        }

        // Add the individual widgets
        if (!tremppi.final)
        {
            tremppi.widgets.push("tools");
        }
        sidebar.nodes.push({
            id: 'widgets', text: 'widgets', expanded: true, group: true,
            nodes: tremppi.widgets.map(function (name) {
                return {id: 'widget+' + name, text: name, selected: (name === tremppi.widget_name)};
            })
        });

        // Add the files in the reporter
        if (typeof tremppi.widget.setup !== 'undefined' && typeof tremppi.widget.setup.files !== 'undefined') {
            sidebar.nodes.push({
                id: 'files', text: 'files', expanded: true, group: true,
                nodes: tremppi.widget.setup.files.map(function (name) {
                    return {id: 'file+' + name, text: name};
                })
            });
        }

        w2ui.layout.content('left', $().w2sidebar(sidebar));
        tremppi.sidebar = w2ui.layout.get('left').content.sidebar;
        tremppi.sidebar.select(tremppi.widget_name);
        tremppi.sidebar.on('*', tremppi.sidebarEvent);
        tremppi.setExit();

        // Set the widget
        tremppi.widget.page();
        w2ui.layout.on('resize', tremppi.widget.layout);

        // Load the specific data
        $.ajaxSetup({cache: false});
        tremppi.getData(tremppi.widget.setData);
    }
    ,
    pickFile: function (filename) {
        $("#select_name").removeAttr('disabled').val(filename);
        $("#rename_btn").removeAttr('disabled');
        $("#delete_btn").removeAttr('disabled');
        tremppi.current_file = tremppi.makeDataFilepath(filename);
        tremppi.current_object = filename;
    },
    sidebarEvent: function (event) {
        var details = event.target.split("+");
        if (event.type === 'click') {
            var work_dir = tremppi.level === 1 ? "../" : "./";
            switch (details[0]) {
                case 'project': // Change project
                    window.open(work_dir + details[1] + "/" + tremppi.widget_name + ".html", "_self");
                    break;

                    // Change widget
                case 'widget':
                    window.open(work_dir + tremppi.project_folder + details[1] + ".html", "_self");
                    break;

                case 'file':
                    tremppi.report.pickData(details[1], 'left');
                    tremppi.pickFile(details[1]);
                    break;
            }
            $("#select_name").val(details[1]);
        } else if (event.type === 'contextMenu') {
            switch (details[0]) {
                case 'file':
                    tremppi.report.pickData(details[1], 'right');
                    tremppi.pickFile(details[1]);
                    break;
            }
            $("#select_name").val(details[1]);
        }
    },
    setExit: function () {
        var make_static = function () {
            $('<div id="static_text">STATIC VERSION</div>').appendTo("#sidebar_field");
        };
        $.ajax({
            type: "GET",
            url: tremppi.getRootAddress(),
            error: function (res) {
                make_static();
            },
            success: function (res) {
                if (res === 'tremmpi browse is running') {
                    $('<button id="rename_btn" onclick="tremppi.exit()" class="btn">EXIT</button>').appendTo("#sidebar_field");
                }
                else {
                    make_static();
                }
            }
        });
    },
    exit: function () {
        $.ajax({
            type: "POST",
            url: tremppi.getProjectAddress() + '?exit',
            error: tremppi.postFail,
            success: function (res) {
                $('body').html('TREMPPI HAS FINISHED');
            }
        });
    },
    delete: function () {
        var old_name = tremppi.current_object;
        $.ajax({
            type: "POST",
            url: tremppi.getProjectAddress() + tremppi.current_file + '?delete',
            error: tremppi.postFail,
            success: function (res) {
                tremppi.sidebar.remove('file+' + old_name);
            }
        });
    },
    rename: function () {
        var old_name = tremppi.current_object;
        var new_name = $("#select_name").val();
        $.ajax({
            type: "POST",
            url: tremppi.getProjectAddress() + tremppi.current_file + '?rename+' + new_name,
            error: tremppi.postFail,
            success: function (res) {
                tremppi.sidebar.insert('files', 'file+' + old_name, {id: 'file+' + new_name, text: new_name});
                tremppi.sidebar.remove('file+' + old_name);
            }
        });
    },
    save: function () {
        tremppi.saveData(tremppi.widget.getData());
    },
    nameValid: function (new_name) {
        if (new_name === '') {
            tremppi.log('No name specified', 'error');
            return false;
        }
        else if (tremppi.projects.indexOf(new_name) !== -1) {
            tremppi.log('The name ' + new_name + ' is already taken', 'error');
            return false;
        }
        else {
            return true;
        }
    },
    newProject: function () {
        var new_name = $("#select_name").val();
        if (tremppi.nameValid(new_name)) {
            location.replace('/' + new_name + '/index.html?init+' + new_name + '+' + Math.random().toString(), "_self");
        }
    },
    cloneProject: function () {
        if (confirm('Do you really want to clone the project ' + tremppi.project_name + '?')) {
            location.replace('/' + tremppi.project_name + '(clone)/index.html?clone+' + tremppi.project_name + '+' + Math.random().toString(), "_self");
        }
    },
    deleteProject: function () {
        if (confirm('Do you really want to delete the project ' + tremppi.project_name + '?')) {
            tremppi.projects.splice(tremppi.projects.indexOf(tremppi.project_name), 1);
            location.replace('/' + tremppi.projects[0] + '/index.html?delete+' + tremppi.project_name + '+' + Math.random().toString(), "_self");
        }
    },
    finalizeProject: function () {
        if (confirm('Do you really want to finalize the project ' + tremppi.project_name + '?')) {
            tremppi.projects.splice(tremppi.projects.indexOf(tremppi.project_name), 1);
            var project = tremppi.level === 1 ? tremppi.project_name : '.';
            location.replace(tremppi.project_folder + '/index.html?finalize+' + project + '+' + Math.random().toString(), "_self");
        }
    },
    renameProject: function () {
        var new_name = $("#select_name").val();
        if (tremppi.nameValid(new_name)) {
            location.replace('/' + new_name + '/index.html?rename+' + tremppi.project_name + '+' + new_name + '+' + Math.random().toString(), "_self");
        }
    },
    postFail: function (res) {
        tremppi.log(res, 'error');
    }
};
// Initial content execution, 
var url = window.location.pathname;
if (url.slice(-1) === '/') {
    url += "index.html";
}
if (location.search !== '') {
    location.replace(window.location.pathname);
}

var url_split = url.split("/");
tremppi.widget_name = url_split[url_split.length - 1].slice(0, -5);
tremppi[tremppi.widget_name] = tremppi.widget = tremppi.widgetInterface();
tremppi.makeHead();
window.onload = tremppi.makeBody;


