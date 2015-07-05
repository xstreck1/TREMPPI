/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi */

tremppi.tools.controls = function () {
    tremppi.toolbar.onClick = tremppi.tools.toolbarClick;
};

tremppi.tools.toolbarClick = function (event) {
};

tremppi.tools.getProgress = function () {
    var url = tremppi.getServerAddress() + "?getProgress";
    $.get(url, function (data) {
        $("progress").html("Progress: " + data);
        if (data !== -1) {
            tremppi.tools.getProgress();
        }
    });
};

tremppi.tools.addToQueue = function (command) {
    var url = tremppi.getServerAddress() + "?tremppi+" + command;
    $.ajax({
        type: "POST",
        url: url,
        success: function (res) {
            tremppi.log(command + " executed successfully.");
            tremppi.tools.getProgress();
        },
        fail: function (res) {
            tremppi.log(command + " failed.");
        }
    });
};