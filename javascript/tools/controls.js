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

tremppi.tools.killAll = function (command) {
    var url = tremppi.getProjectAddress() + "?killAll";
    $.ajax({
        type: "POST",
        url: url,
        success: function (res) {
            tremppi.log("Kill all executed successfully.");
            tremppi.tools.getCommands();
        },
        fail: function (res) {
            tremppi.log("Kill all failed.", 'error');
        }
    });
};

tremppi.tools.addToQueue = function (command) {
    var url = tremppi.getProjectAddress() + "?tremppi+" + command;
    if (command === 'clean'){
        if (!confirm('Do you really want to call ' + command + '? This operation is irreversible.')) {
            return;
        }
    }
    $.ajax({
        type: "POST",
        url: url,
        success: function (res) {
            tremppi.log(command + " executed successfully.");
            tremppi.tools.getCommands();
        },
        fail: function (res) {
            tremppi.log(command + " failed.", 'error');
        }
    });
};