/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global w2ui */

tremppi = {
    server_port: 8080,
    server_location: "localhost",
    getServerAddress: function () {
        return "http://" + this.server_location + ":" + this.server_port + "/";
    },
    getDataFile: function(file) {
        return "./data/" + tremppi.widget_name + "/" + file + ".json";
    },
    widgetInterface: function () {
        return {
            setPage: function () { tremppi.log("setPage not implemented", "warning" ); },
            setData: function (data) { tremppi.log("setData not implemented", "warning" ); return false; },
            layout: function () { tremppi.log("layout not implemented", "warning" ); },
            save: function () { tremppi.log("save not implemented", "warning" ); },
            setDefaultData: function (data) { tremppi.log("setDefaultDatas not implemented", "warning" ); },
            defaultToolbar: function (){ tremppi.log("toolbar not implemented", "warning" ); }
        };
    },
    // the setup function - sets globals, obtains data and adds server content, if possible
    initiate: function () {
        var url = window.location.pathname;
        tremppi.widget_name = url.substring(url.lastIndexOf('/') + 1).slice(0, -5);
        tremppi[tremppi.widget_name] = tremppi.widget = this.widgetInterface();
        tremppi.data_file = "./" + tremppi.widget_name + ".json";
        tremppi.makeHead();
        $(window).ready(tremppi.construct);
    },
    // Saves the variable to the respective file
    save: function () {
        var content = JSON.stringify(tremppi.data, null, '\t');
        var url = tremppi.getServerAddress() + "?" + tremppi.data_file;
        $.ajax({
            type: "POST",
            url: url,
            data: content,
            success: function (res) {
                tremppi.log(tremppi.widget_name + " saved successfully.");
            },
            fail: function (res) {
                tremppi.log("DATA NOT SAVED!");
            }
        });
    },
    load: function (data) {
        // load data
        tremppi.data = data;
        // add default content, if original not available
        if (typeof (tremppi.data.configured) === 'undefined' || tremppi.data.configured === false) {
            tremppi.widget.setDefaultData(data);
            tremppi.data.configured = true;
            tremppi.save();
        }
        if (tremppi.widget.setData(data))
            tremppi.log(tremppi.widget_name + " loaded.");
    },
    construct: function () {
        tremppi.setPage();
        tremppi.widget.setPage();
        w2ui.layout.on('resize', tremppi.widget.layout);

        // Force fresh load
        $.ajaxSetup({cache: false});
        // get data
        $.getJSON(tremppi.data_file, tremppi.load);
        // Reinstate
        // $.ajaxSetup({cache: true});
    },
    setPage: function () {
        var sidebar = {
            name: 'sidebar',
            nodes: [
                {id: 'widget_list', text: 'widgets', expanded: true, group: true,
                    nodes: [
                        {id: 'editor', text: 'editor'},
                        {id: 'select', text: 'select'},
                        {id: 'properties', text: 'properties'},
                        {id: 'summary', text: 'summary'}
                    ]
                }
            ]
        };
        var layout_style = 'border: 0px solid #dfdfdf;';
        var layout = {
            name: 'layout',
            panels: [
                {type: 'left', style: layout_style, size: 200, content: '<div id="files" ></div>'},
                {type: 'main', style: layout_style, content: '<div id="widget" ></div>'},
                {type: 'bottom', size: 20, content: '<div id="log_line" ></div>'}
            ]
        };
        
        var toolbar = tremppi.widget.defaultToolbar();
        if (toolbar !== "undefined")
            layout.panels[1].toolbar = toolbar;
        
        // Layout
        $('body').w2layout(layout);
        // Toolbar
        tremppi.widget.toolbar = w2ui.layout.get("main").toolbar;

        // Side bar
        w2ui.layout.content('left', $().w2sidebar(sidebar));
        var sidebar = w2ui.layout.get('left').content.sidebar;
        sidebar.select(tremppi.widget_name);
        sidebar.on('click', function (event) {
            window.open("./" + event.target + ".html", "_self");
        });
    },
    makeHead: function () {
        $("head").append(
                '<title>TREMPPI ' + tremppi.widget_name + '</title>' +
                '<meta charset=utf-8 />' +
                '<link rel="icon" type="image/x-icon" href="./favicon.ico">');
    }
};
tremppi.initiate();


