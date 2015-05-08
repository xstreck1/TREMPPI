/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global w2ui */

tremppi = {
    server_port: 8080,
    server_location: "localhost",
    project_name: "default_project_name",
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
            }
        };
    },
    getServerAddress: function () {
        return "http://" + this.server_location + ":" + this.server_port + "/";
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
        var url = tremppi.getServerAddress() + "?" + tremppi.makeDataFilepath(filename);
        $.ajax({
            type: "POST",
            url: url,
            data: content,
            success: function (res) {
                tremppi.log(filename + " saved successfully.");
            },
            fail: function (res) {
                tremppi.log("Load of the file " + filename + " failed! Data were not saved!");
            }
        });
    },
    getData: function (callback, filename) {
        $.getJSON(tremppi.makeDataFilepath(filename), callback)
                .done(function () {
                    tremppi.log(filename + " loaded successfully.");
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
            localStorage.setItem(makeStorageKey(key), value);
        }
    },
    getItem: function (key, value) {
        if (typeof localStorage === 'undefined') {
            tremppi.log("localStorage not available, browser key " + key + " not loaded", "warning");
            return null;
        }
        else {
            return localStorage.getItem(makeStorageKey(key));
        }
    },
    makeHead: function () {
        $("head").append(
                '<meta charset=utf-8 />' +
                '<link rel="icon" type="image/x-icon" href="./favicon.ico">');
        document.title = tremppi.widget_name;
    },
    makeBody: function () {
        var sidebar = {
            name: 'sidebar',
            nodes: [
                {id: 'widget_list', text: 'widgets', expanded: true, group: true,
                    nodes: [
                        {id: 'index', text: 'index'},
//                        {id: 'editor', text: 'editor'},
//                        {id: 'select', text: 'select'},
//                        {id: 'properties', text: 'properties'},
//                        {id: 'quantitative', text: 'quantitative'},
//                        {id: 'qualitative', text: 'qualitative'}
                    ]
                }
            ]
        };

        // Set basic layout
        var layout_style = 'border: 0px solid #dfdfdf;';
        var layout = {
            name: 'layout',
            panels: [
                {type: 'left', style: layout_style, size: 200, content: '<div id="files" ></div>'},
                {type: 'main', style: layout_style, content: '<div id="widget" ></div>'},
                {type: 'bottom', size: 20, content: '<div id="log_line" ></div>'}
            ]
        };
        $('body').w2layout(layout);

        // Set left side bar
        w2ui.layout.content('left', $().w2sidebar(sidebar));
        var sidebar = w2ui.layout.get('left').content.sidebar;
        sidebar.select(tremppi.widget_name);
        sidebar.on('click', function (event) {
            window.open("./" + event.target + ".html", "_self");
        });

        // Set the toolbar, if any
        if (toolbar !== "undefined")
            layout.panels[1].toolbar = tremppi.widget.toolbar;
        tremppi.toolbar = w2ui.layout.get("main").toolbar;

        // Set the widget
        tremppi.widget.page();
        w2ui.layout.on('resize', tremppi.widget.layout);

        // Load the specific data
        tremppi.getData(tremppi.widget.setData);
    }
};
// Initial content execution, 
$.ajaxSetup({cache: false});
var url = window.location.pathname;
tremppi.widget_name = url.substring(url.lastIndexOf('/') + 1).slice(0, -5);
tremppi[tremppi.widget_name] = tremppi.widget = tremppi.widgetInterface();
tremppi.makeHead();
$(window).ready(tremppi.makeBody);


