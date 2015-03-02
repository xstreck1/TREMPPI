/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi = {
    server_port: 8080,
    server_location: "localhost",
    getServerAddress: function () {
        return "http://" + this.server_location + ":" + this.server_port + "/";
    },
    // the setup function - sets globals, obtains data and adds server content, if possible
    initiate: function () {
        var url = window.location.pathname;
        tremppi.widget_name = url.substring(url.lastIndexOf('/') + 1).slice(0, -5);
        tremppi[tremppi.widget_name] = tremppi.widget = {};
        tremppi.data_file = tremppi.widget_name + ".json";
        tremppi.makeHead();
        $(window).ready(tremppi.load);
        $(window).resize(tremppi.layout);
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
            }
        });
    },
    load: function () {
        // Force fresh load
        $.ajaxSetup({ cache: false });
        // get data
        $.getJSON(tremppi.data_file, function (data) {
            // load data
            tremppi.data = data;
            // add default content, if original not available
            if (typeof (tremppi.data.configured) === 'undef' || tremppi.data.configured === false) {
                $.extend(true, tremppi.data, tremppi.widget.default);
                tremppi.data.configured = true;
            }
            tremppi.widget.widgetJS();
            tremppi.widget.controlsJS();
        });
    },
    layout: function () {
        tremppi.page.layout();
        tremppi.widget.layout();
    },
    makeHead: function () {
        $("head").append(
                '<title>TREMPPI ' + tremppi.widget_name + '</title>' +
                '<meta charset=utf-8 />' +
                '<link rel="icon" type="image/x-icon" href="./favicon.ico">');
    }
};
tremppi.initiate();


