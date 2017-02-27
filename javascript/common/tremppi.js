/******************************************************************************
 Created by Adam Streck, 2013-2015, adam.streck@fu-berlin.de
 
 This file is part of the Toolkit for Reverse Engineering of Molecular Pathways
 via Parameter Identification (TREMPPI)
 
 This program is free software: you can redistribute it and/or modify it under
 the terms of the GNU General Public License as published by the Free Software
 Foundation, either version 3 of the License, or (at your option) any later
 version.
 
 This program is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License along with
 this program.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

/* global w2ui tremppi */

// TODO Project controls target (currently on selected, not clicked)

tremppi = {
    widgets: ['index', 'editor', 'properties', 'select', 'quantitative', 'qualitative', 'regulations', 'correlations', 'witness', 'group', 'tools'],
    reports: ['quantitative', 'qualitative', 'regulations', 'correlations', 'witness', 'group'],
    hasControls: function () {
        return !(typeof tremppi.exec_type === 'undefined' || tremppi.exec_type === 'static');
    },
    isFinal: function () {
        return typeof tremppi.setup.final === 'undefined' ? false : tremppi.setup.final;
    },
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
            },
            beforeUnload: function () {
                tremppi.log("beforeUnload not implemented", "warning");
            }
        };
    },
    getHostAddress: function () {
        return window.location.protocol + "//" + window.location.host;
    },
    getRootAddress: function () {
        var path = window.location.pathname;
        var directory = path.substring(0, path.lastIndexOf('/') + 1);
        if (tremppi.level === 1) {
            directory += "../";
        }
        return tremppi.getHostAddress() + directory;
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
        var url = tremppi.getProjectAddress() + tremppi.makeDataFilepath(filename) + "?command=save";
        $.ajax({
            type: "POST",
            url: url,
            data: content,
            contentType: 'application/json',
            success: tremppi.log,
            error: tremppi.logError
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
        } else {
            localStorage.setItem(tremppi.makeStorageKey(key), value);
        }
    },
    getItem: function (key, default_value) {
        if (typeof localStorage === 'undefined') {
            tremppi.log("localStorage not available, browser key " + key + " not loaded", "warning");
            return null;
        } else {
            var value = localStorage.getItem(tremppi.makeStorageKey(key));
            if (value === null) {
                if (typeof default_value === 'undefined') {
                    return null;
                } else {
                    return default_value;
                }
            } else {
                return value;
            }
        }
    },
    setPage: function () {
        var url = tremppi.getProjectAddress() + tremppi.widget_name + ".html?command=page";
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
        for (var i = 0; i < tremppi.widgets.length; i++) {
            head.appendChild(tremppi.makeScript('./data/' + tremppi.widgets[i] + '.js?_=' + Math.random().toString().slice(2)));
        }

        document.title = tremppi.widget_name;
    },
    configure: function () {
        if (typeof tremppi.projects === 'undefined') {
            tremppi.project_name = tremppi.setup.project_name;
            tremppi.projects = [tremppi.setup.project_name];
            tremppi.project_folder = "";
            tremppi.level = 0;
        } else {
            tremppi.project_name = tremppi.setup.project_name;
            tremppi.project_folder = tremppi.project_name + "/";
            tremppi.level = 1;
        }
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
                {type: 'top', style: layout_style, size: 40, content: '<div id="top_panel"></div>'},
                {type: 'left', style: layout_style, size: 200, content: '<div id="files" ></div>'},
                {type: 'main', style: layout_style, content: '<div id="widget" ></div>', toolbar: tremppi.widget.toolbarClass()},
                {type: 'bottom', size: 20, content: '<div id="log_line" ></div>'}
            ]
        };
        tremppi.layout = $('body').w2layout(layout);
        tremppi.toolbar = w2ui.layout.get("main").toolbar;
        // Distable toolbar for final projects
        if (tremppi.toolbar.items.length === 0) {
            w2ui.layout.hideToolbar('main');
        }

        var user_controls =
                '<button class="btn" id="exit_btn" onclick="tremppi.models()" >Public Models</button><button class="btn" id="exit_btn" onclick="tremppi.docs()" >Documentation</button>';
        var tremppi_controls = "";
        if (!tremppi.hasControls()) {
            user_controls += '<span id="static_text">STATIC VERSION</span>';
        } else if (tremppi.exec_type === 'browse')
        {
            user_controls += '<button class="btn" id="exit_btn" onclick="tremppi.exit()" >EXIT</button>';
        } else if (tremppi.exec_type === 'server')
        {
            user_controls += '<button id="user_btn" onclick="tremppi.userBtn()" class="btn">USER</button>';
            user_controls += '<button id="exit_btn" onclick="tremppi.logOutBtn()" class="btn">LOG OUT</button>';
        } else
        {
            user_controls += '<div id="static_text">UNKNOWN SERVER TYPE</div>';
            tremppi.log('unknown exec_type ' + tremppi.exec_type, 'error');
        }

        $("#top_panel").html(
                '<img id="logo" src="logo.png" />' +
                '<span id="tremppi_controls">' +
                tremppi_controls +
                '</span>' +
                '<span id="user_controls">' +
                user_controls +
                '</span>'
                );
        var sidebar = {
            name: 'sidebar',
            nodes: [],
            topHTML: "",
            bottomHTML: ""
        };
        if (tremppi.hasControls()) {
            sidebar.menu = [
                {id: 0, text: 'new project'},
                {id: 1, text: 'open'},
                {id: 2, text: 'compare'},
                {id: 3, text: 'clone'},
                {id: 4, text: 'rename'},
                {id: 5, text: 'delete'},
                {id: 6, text: 'finalize'},
                {id: 7, text: 'save'},
                {id: 8, text: 'download'},
                {id: 9, text: 'upload'}
            ];
        }
        else {
            sidebar.menu = [
                {id: 1, text: 'open'},
                {id: 2, text: 'compare'}
            ];
        }
        // Add the projects
        sidebar.nodes.push({
            id: 'projects',
            text: 'My Projects',
            expanded: true,
            group: true,
            nodes: tremppi.projects.map(function (proj_name) {
                var project_node = {
                    id: 'project+' + proj_name,
                    text: proj_name,
                    img: 'icon-folder',
                };
                if (proj_name === tremppi.project_name) {
                    project_node.expanded = true;
                    project_node.selected = true;
                    project_node.nodes = tremppi.widgets.map(function (widget_name) {
                        var widget_node = {
                            id: 'widget+' + widget_name,
                            text: widget_name,
                            img: tremppi.reports.indexOf(widget_name) === -1 ? 'icon-page' : 'icon-folder'
                        };
                        if (typeof tremppi[widget_name].setup !== 'undefined' && typeof tremppi[widget_name].setup.files !== 'undefined') {
                            if (widget_name === tremppi.widget_name) {
                                widget_node.nodes = tremppi.widget.setup.files.map(function (file_name) {
                                    return {
                                        id: 'file+' + file_name + '+' + widget_name,
                                        text: file_name,
                                        img: 'icon-page'
                                    };
                                });
                            }
                            widget_node.count = tremppi[widget_name].setup.files.length;
                        }
                        if (widget_name === tremppi.widget_name) {
                            widget_node.expanded = true;
                            widget_node.selected = true;
                        }
                        return widget_node;
                    });
                }
                return project_node;
            })
        });

        w2ui.layout.content('left', $().w2sidebar(sidebar));
        tremppi.sidebar = w2ui.layout.get('left').content.sidebar;
        tremppi.sidebar.select(tremppi.widget_name);
        tremppi.sidebar.on('*', tremppi.sidebarEvent);
        // Set the widget
        tremppi.widget.page();
        w2ui.layout.on('resize', tremppi.widget.layout);
        // Load the specific data
        $.ajaxSetup({cache: false});
        tremppi.getData(tremppi.widget.setData);
        window.onbeforeunload = tremppi.widget.beforeUnload;
    },
    pickFile: function (filename) {
        tremppi.current_file = tremppi.makeDataFilepath(filename);
        tremppi.current_object = filename;
    },
    activateMenuItems: function () {
        for (var i = 0; i < arguments.length; i++) {
            var argument = arguments[i];
            var item = tremppi.sidebar.menu.find(function (object) {
                return object.text === argument;
            });
            if (typeof item !== 'undefined') {
                item.hidden = false;
            }
        }
    },
    sidebarEvent: function (event) {
        var details = event.target.split("+");
        if (event.type === 'click') {
            switch (details[0]) {
                case 'project': // Change project
                    if (tremppi.level === 1) {
                        window.open(tremppi.getRootAddress() + details[1] + "/" + tremppi.widget_name + ".html", "_self");
                    }
                    break;
                    // Change widget
                case 'widget':
                    window.open(tremppi.getRootAddress() + tremppi.project_folder + details[1] + ".html", "_self");
                    break;
                case 'file':
                    tremppi.widget.leftSelected = true; // We have selected something
                    tremppi.report.showPanel('left');
                    tremppi.report.pickData(details[1], 'left');
                    tremppi.pickFile(details[1]);
                    break;
            }
        } else if (event.type === 'contextMenu') {
            // hide all buttons first, then show the relevant ones
            for (var i = 0; i < tremppi.sidebar.menu.length; i++) {
                tremppi.sidebar.menu[i].hidden = true;
            }

            switch (details[0]) {
                case 'projects':
                    if (tremppi.level === 1) {
                        tremppi.activateMenuItems('new project');
                        tremppi.activateMenuItems('upload');
                    }
                    break;
                case 'project':
                    if (tremppi.level === 1) {
                        tremppi.activateMenuItems('open', 'download', 'clone', 'rename', 'delete');
                    }
                    if (!tremppi.isFinal()) {
                        tremppi.activateMenuItems('finalize');
                    }

                    break;
                case 'widget':
                    tremppi.activateMenuItems('open');
                    if (!tremppi.isFinal() && (details[1] === tremppi.widget_name) && tremppi.reports.indexOf(tremppi.widget_name) === -1 && ("tools" !== tremppi.widget_name)) {
                        tremppi.activateMenuItems('save');
                    }
                    break;
                case 'file':
                    tremppi.activateMenuItems('open', 'rename', 'delete');
                    if (tremppi.widget.leftSelected) {
                        tremppi.activateMenuItems('compare');
                    }
                    break;
            }
        } else if (event.type === 'menuClick') {
            switch (details[0]) {
                case 'projects':
                    switch (event.menuItem.text) {
                        case 'new project':
                            tremppi.new_project();
                            break;
                        case 'upload':
                            tremppi.dialogPopup = w2popup.open({
                                body: '<div class="tremppi_popup">\n\
                                            <div class="popup_title">UPLOAD PROJECT</div>\n\
                                            <form enctype="multipart/form-data" method="post" name="fileinfo"> \n\
                                                <input id="fileSelect" type="file" accept=".zip" required />\n\
                                            </form>\n\
                                           <BUTTON onClick="tremppi.uploadProject()" class="btn" id="submitFile">SUBMIT</BUTTON>\n\
                                        </div>',
                                width: 350,
                                height: 175,
                                showClose: true
                            });
                            break;
                    }
                    break;
                case 'project': // Change project
                    switch (event.menuItem.text) {
                        case 'open':
                            if (tremppi.level === 1) {
                                window.open(tremppi.getRootAddress() + details[1] + "/" + tremppi.widget_name + ".html", "_self");
                            }
                            break;
                        case 'delete':
                            tremppi.deleteProject();
                            break;
                        case 'rename':
                            tremppi.renameProject();
                            break;
                        case 'clone':
                            tremppi.cloneProject();
                            break;
                        case 'finalize':
                            tremppi.finalizeProject();
                            break;
                        case 'download':
                            tremppi.downloadProject();
                            break;
                    }
                    break;
                    // Change widget
                case 'widget':
                    switch (event.menuItem.text) {
                        case 'open':
                            window.open(tremppi.getRootAddress() + tremppi.project_folder + details[1] + ".html", "_self");
                            break;
                        case 'save':
                            tremppi.save();
                            break;
                    }
                    break;
                case 'file':
                    switch (event.menuItem.text) {
                        case 'open':
                            tremppi.report.pickData(details[1], 'left');
                            tremppi.widget.leftSelected = true;
                            tremppi.pickFile(details[1]);
                            break;
                        case 'compare':
                            tremppi.report.showAll();
                            tremppi.report.pickData(details[1], 'right');
                            tremppi.pickFile(details[1]);
                            break;
                        case 'delete':
                            tremppi.delete(details[1]);
                            break;
                        case 'rename':
                            tremppi.rename(details[1]);
                            break;
                    }
                    break;
            }
        }
    }
    ,
    docs: function () {
        window.open('http://dibimath.github.io/TREMPPI/');
    },
    models: function () {
        window.open('http://dibimath.github.io/TremppiDemo/index.html');
    },
    exit: function () {
        $.ajax({
            type: "POST",
            url: tremppi.getRootAddress() + 'exit',
            error: tremppi.logError,
            success: function (res) {
                $('body').html('TREMPPI HAS FINISHED');
            }
        });
    },
    logOutBtn: function () {
        location.replace(tremppi.getRootAddress() + 'user/sign-out', "_self");
    },
    userBtn: function () {
        location.replace(tremppi.getRootAddress() + 'user/profile', "_self");
    },
    delete: function (selectedFile) {
        $.ajax({
            type: "POST",
            url: tremppi.getProjectAddress() + tremppi.makeDataFilepath(selectedFile) + '?command=delete&type=file',
            error: tremppi.logError,
            success: function (res) {
                tremppi.sidebar.remove('file+' + selectedFile + '+' + tremppi.widget_name);
            }
        });
    },
    fileNameValid: function (new_name) {
        if (new_name === '') {
            tremppi.log('No name specified', 'error');
            return false;
        } else if (tremppi.sidebar.find({text: new_name}).length !== 0) {
            tremppi.log('The name ' + new_name + ' is already taken', 'error');
            return false;
        } else if (new_name.indexOf(' ') >= 0) {
            tremppi.log('No whitespace allowed', 'error');
            return false;
        } else {
            return true;
        }
    },
    rename: function (selectedFile) {
        var new_name = prompt("Please enter a new name for the file.", selectedFile);
        if (new_name !== null && tremppi.fileNameValid(new_name)) {
            $.ajax({
                type: "POST",
                url: tremppi.getProjectAddress() + tremppi.makeDataFilepath(selectedFile) + '?command=rename&type=file&new_name=' + new_name,
                error: tremppi.logError,
                success: function (res) {
                    tremppi.sidebar.insert('widget+' + tremppi.widget_name, 'file+' + selectedFile + '+' + tremppi.widget_name, 
                    {id: 'file+' + new_name + '+' + tremppi.widget_name, text: new_name, img: 'icon-page'});
                    tremppi.sidebar.remove('file+' + selectedFile + '+' + tremppi.widget_name);
                    tremppi.log(res);
                }
            });
        }
    },
    new_project: function () {
        var new_name = prompt("Please enter the name for the new project.", "");
        if (new_name !== null && tremppi.projectNameValid(new_name)) {
            $.ajax({
                type: "POST",
                url: tremppi.getProjectAddress() + tremppi.current_object + '?command=new_project&new_name=' + new_name,
                success: function (res) {
                    location.replace(tremppi.getRootAddress() + new_name + '/' + tremppi.current_object + '.html', "_self");
                    tremppi.log(res);
                },
                error: tremppi.logError
            });
        }
    },
    save: function () {
        tremppi.saveData(tremppi.widget.getData());
    },
    projectNameValid: function (new_name) {
        if (new_name === '') {
            tremppi.log('No name specified', 'error');
            return false;
        } else if (tremppi.projects.indexOf(new_name) !== -1) {
            tremppi.log('The name ' + new_name + ' is already taken', 'error');
            return false;
        } else if (new_name.indexOf(' ') >= 0) {
            tremppi.log('No whitespace allowed', 'error');
            return false;
        } else {
            return true;
        }
    },
    cloneProject: function () {
        if (confirm('Do you really want to clone the project ' + tremppi.project_name + '?')) {
            $.ajax({
                type: "POST",
                url: tremppi.getProjectAddress() + tremppi.current_object + '.html?command=clone&type=folder&name=' + tremppi.project_name,
                success: function (res) {
                    location.replace(tremppi.getRootAddress() + tremppi.project_name + '(clone)/' + tremppi.current_object + '.html', "_self");
                    tremppi.log(res);
                },
                error: tremppi.logError
            });
        }
    },
    deleteProject: function () {
        if (tremppi.projects.length <= 1) {
            tremppi.log('Trying to delete the only project. At least one TREMPPI project must exists at any time.', 'error');
        } else if (confirm('Do you really want to delete the project ' + tremppi.project_name + '?')) {
            $.ajax({
                type: "POST",
                url: tremppi.getProjectAddress() + tremppi.current_object + '.html?command=delete&type=folder',
                success: function (res) {
                    location.replace(tremppi.getRootAddress() + tremppi.projects[0] + '/' + tremppi.current_object + '.html', "_self");
                    tremppi.log(res);
                },
                error: tremppi.logError
            });
        }
    },
    downloadProject: function () {
        if (confirm('Do you really want to download the project ' + tremppi.project_name + '?')) {
            $.ajax({
                type: "POST",
                url: tremppi.getProjectAddress() + tremppi.current_object + '.html?command=download&type=folder',
                success: function (res) {
                    window.location = tremppi.getRootAddress() + tremppi.project_name + '.zip';
                },
                error: tremppi.logError
            });
        }
    },
    uploadProject: function () {
        data = new FormData();
        var file = $('#fileSelect')[0].files[0];
        if (typeof file === 'undefined')
        {
            tremppi.logError("No file selected");
        } else {
            data.append('file', file);
            var oReq = new XMLHttpRequest();
            oReq.open("POST", tremppi.getProjectAddress() + tremppi.current_object + '.html?command=upload&type=folder', true);
            oReq.onload = function (oEvent) {
                if (oReq.status === 200) {
                    tremppi.log("Project uploaded. " + oReq.responseText);
                    location.reload(true);
                } else {
                    tremppi.logError("Error " + oReq.responseText + " occurred when trying to upload the project.");
                }
            };
            oReq.send(data);
        }
        tremppi.dialogPopup.close();
    },
    finalizeProject: function () {
        if (confirm('Do you really want to finalize the project ' + tremppi.project_name + '?')) {
            tremppi.projects.splice(tremppi.projects.indexOf(tremppi.project_name), 1);
            var project = tremppi.level === 1 ? tremppi.project_name : '.';
            $.ajax({
                type: "POST",
                url: tremppi.getProjectAddress() + tremppi.current_object + '.html?command=finalize',
                success: function (res) {
                    location.reload(true);
                    tremppi.log(res);
                },
                error: tremppi.logError
            });
        }
    }
    ,
    renameProject: function () {
        var new_name = prompt("Please enter the new name for the project.", tremppi.project_name);
        if (new_name !== null && tremppi.projectNameValid(new_name)) {
            $.ajax({
                type: "POST",
                url: tremppi.getProjectAddress() + tremppi.current_object + '.html?command=rename&type=folder&new_name=' + new_name,
                success: function (res) {
                    location.replace(tremppi.getRootAddress() + new_name + '/tools.html', "_self");
                    tremppi.log(res);
                },
                error: tremppi.logError
            });
        } else {
            tremppi.log('Invalid project name ' + new_name, 'error');
        }
    }
};
// Initial content execution, 
var url = window.location.pathname;
if (url.slice(-1) === '/') {
    url += "index.html";
}
// Remove the query string
if (location.search !== '') {
    location.replace(window.location.pathname);
}

for (var i = 0; i < tremppi.widgets.length; i++) {
    tremppi[tremppi.widgets[i]] = {};
}
var url_split = url.split("/");
tremppi.widget_name = url_split[url_split.length - 1].slice(0, -5);
tremppi[tremppi.widget_name] = tremppi.widget = tremppi.widgetInterface();
if (tremppi.reports.indexOf(tremppi.widget_name) !== -1) {
    tremppi.widget.leftSelected = false;
}
tremppi.makeHead();
window.onload = tremppi.makeBody;


