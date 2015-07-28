/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global w2ui */

tremppi = {
    widgets: ['index', 'editor', 'select', 'properties', 'quantitative', 'qualitative', 'regulations', 'correlations', 'witness', 'tools'],
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
    getServerAddress: function () {
        return "http://" + tremppi.setup.server_location + ":" + tremppi.setup.server_port + "/" + tremppi.project_folder;
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
        var url = tremppi.getServerAddress() + tremppi.makeDataFilepath(filename) + "?save";
        $.ajax({
            type: "POST",
            url: url,
            data: content,
            success: function (res) {
                if (typeof filename === 'undefined') {
                    tremppi.log(tremppi.widget_name + " saved successfully.");
                } else {
                    tremppi.log(filename + " saved successfully.");
                }
            },
            fail: function (res) {
                tremppi.log("Load of the file " + filename + " failed! Data were not saved!");
            }
        });
    },
    getData: function (callback, filename) {
        $.getJSON(tremppi.makeDataFilepath(filename), callback)
                .done(function () {
                    if (typeof filename === 'undefined') {
                        tremppi.log(tremppi.widget_name + " loaded successfully.");
                    } else {
                        tremppi.log(filename + " loaded successfully.");
                    }
                })
                .fail(function (jqXHR, textStatus, errorThrown) {
                    tremppi.log("Load of the file " + filename + " failed!");
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
        head.appendChild(tremppi.makeScript('./configure.js?_=' + Math.random().toString().slice(2))); // load the setup with cache busing
        head.appendChild(tremppi.makeScript('../projects.js?_=' + Math.random().toString().slice(2))); // load the setup with cache busing
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
        tremppi.current_file = tremppi.widget_name;
    },
    makeBody: function () {
        tremppi.configure();

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
        if (tremppi.toolbar.items.length === 0) {
            w2ui.layout.hideToolbar('main');
        }

        var select_name = '"' + (tremppi.widget_name === 'index' ? tremppi.project_name : tremppi.widget_name) + '"';
        var file_manip_dis = tremppi.widget_name === 'index' && tremppi.level === 1? '' : 'disabled';
        // Set left side bar
        var project_controls = tremppi.level === 0 ? '' : 
                    '<input id="select_name" type="text" name="Fill to create or rename" value=' + select_name + file_manip_dis + '>' +
                    '<button id="rename_btn" onclick="tremppi.rename()" class="btn" ' + file_manip_dis + '>RENAME</button>' +
                    '<button id="clone_btn" onclick="tremppi.clone()" class="btn"' + file_manip_dis + '>CLONE</button>' +
                    '<button id="delete_btn" onclick="tremppi.delete()" class="btn" ' + file_manip_dis + '>DELETE</button>';
        var sidebar = {
            name: 'sidebar',
            nodes: [],
            topHTML: 
                    '<div class="sidebar_field">' +
                    '<img id="logo" src="logo.png" />' +
                    '</div>',
            bottomHTML: 
                    '<div class="sidebar_field">' +                
                    '<button id="save_btn" onclick="tremppi.save()" class="btn">SAVE</button>' +
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

        // Set the widget
        tremppi.widget.page();
        w2ui.layout.on('resize', tremppi.widget.layout);

        // Load the specific data
        $.ajaxSetup({cache: false});
        tremppi.getData(tremppi.widget.setData);
    },
    pickFile: function(filename) {
        $("#select_name").removeAttr('disabled').val(filename);
        $("#rename_btn").removeAttr('disabled');
        $("#delete_btn").removeAttr('disabled');
        tremppi.current_file = tremppi.makeDataFilepath(filename);
    },
    sidebarEvent: function (event) {
        var details = event.target.split("+");
        if (event.type === 'click') {
            switch (details[0]) {
                case 'project': // Change project
                    window.open("/" + details[1] + "/" + tremppi.widget_name + ".html", "_self");
                    break;

                    // Change widget
                case 'widget':
                    window.open("/" + tremppi.project_folder + details[1] + ".html", "_self");
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
    save: function () {
        var data = tremppi.widget.getData();
        var content = JSON.stringify(data, null, '\t');
        tremppi.tremppiPost(tremppi.makeDataFilepath(), 'save', content);
    },
    clone: function () {
        tremppi.tremppiPost(tremppi.current_file, 'clone'); 
    },
    delete: function () {
        tremppi.tremppiPost(tremppi.current_file, 'delete');
    },
    rename: function () {
        var new_name = $("#select_name").val();
        tremppi.tremppiPost(tremppi.current_file, 'rename+' + new_name);
    },
    postSucces: function(res) {
        window.open(tremppi.project_folder + tremppi.widget_name + ".html", "_self");
    },
    postFail: function(res) {
        tremppi.log(res, 'error');
    },
    tremppiPost: function(path, command, data) {
        var request = {
            type: "POST",
            url: tremppi.getServerAddress() + path + '?' + command,
            fail: tremppi.postFail
        };
        if (typeof data !== 'undefined') {
            request.data = data;
            request.success = tremppi.log;
        }
        else {
            request.success = tremppi.postSuccess;
        }
        $.ajax(request);
    }
};
// Initial content execution, 
var url = window.location.pathname;
if (url.slice(-1) === '/') {
    url += "index.html";
}

var url_split = url.split("/");
tremppi.widget_name = url_split[url_split.length - 1].slice(0, -5);
tremppi[tremppi.widget_name] = tremppi.widget = tremppi.widgetInterface();
tremppi.makeHead();
window.onload = tremppi.makeBody;


