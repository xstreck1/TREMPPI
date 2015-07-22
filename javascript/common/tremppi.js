/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global w2ui */

tremppi = {
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
        return "http://" + tremppi.setup.server_location + ":" + tremppi.setup.server_port + "/";
    },
    makeDataFilepath: function (filename) {
        if (typeof filename === 'undefined') {
            return "data/" + tremppi.widget_name + ".json";
        } else {
            return "data/" + tremppi.widget_name + "/" + filename + ".json";
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
    makeHead: function (level) {
        var head = document.getElementsByTagName('head')[0];

        // common 
        head.appendChild(tremppi.makeLink('icon', 'image/x-icon', './favicon.ico'));
        var configure_str = level === 0 ? "./configure.js" : "../configure.js";
        head.appendChild(tremppi.makeScript(configure_str + '?_=' + Math.random().toString().slice(2))); // load the setup with cache busing
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
    makeBody: function () {
        if (tremppi.setup.level === 0) {
            tremppi.project_name = tremppi.setup.project_name;
            tremppi.projects_name = [tremppi.setup.project_name];
            tremppi.project_folder = "";
        }
        else if (tremppi.setup.level === 1) {
            tremppi.project_name = window.location.pathname.split("/")[1];
            tremppi.project_folder = tremppi.project_name + "/";
            tremppi.projects_name = tremppi.setup.projects_name;
            console.log(tremppi.project_folder);
        }
        else {
            tremppi.log("unknown project level " + tremppi.setup.level, "error");
        }

        var sidebar = {
            name: 'sidebar',
            nodes: [],
            topHTML: '<img id="logo" src="logo.png" />'
        };

        for (var i = 0; i < tremppi.projects_name.length; i++) {
            var project_location = tremppi.setup.level === 0 ? '/' :  '/' + tremppi.projects_name[i] + '/';
            sidebar.nodes.push({
                id: tremppi.projects_name[i], text: tremppi.projects_name[i], expanded: true, group: true,
                nodes: [
                    {id: project_location + 'index', text: 'index'},
                    {id: project_location + 'editor', text: 'editor'},
                    {id: project_location + 'select', text: 'select'},
                    {id: project_location + 'properties', text: 'properties'},
                    {id: project_location + 'quantitative', text: 'quantitative'},
                    {id: project_location + 'qualitative', text: 'qualitative'},
                    {id: project_location + 'regulations', text: 'regulations'},
                    {id: project_location + 'correlations', text: 'correlations'},
                    {id: project_location + 'witness', text: 'witness'},
                    {id: project_location + 'tools', text: 'tools'}
                ],
                hidden: tremppi.projects_name[i] !== tremppi.project_name 
            });
        }

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

        // Set left side bar
        w2ui.layout.content('left', $().w2sidebar(sidebar));
        var sidebar = w2ui.layout.get('left').content.sidebar;
        sidebar.select(tremppi.widget_name);
        sidebar.on('click', function (event) {
            window.open(event.target + ".html", "_self");
        });

        // Set the widget
        tremppi.widget.page();
        w2ui.layout.on('resize', tremppi.widget.layout);

        // Load the specific data
        $.ajaxSetup({cache: false});
        tremppi.getData(tremppi.widget.setData);
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
tremppi.makeHead(url_split.length - 2);
window.onload = tremppi.makeBody;


