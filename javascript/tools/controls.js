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
    if (event.target === 'spawn') {
        tremppi.tools.addToQueue(event.target);
    }
};

tremppi.tools.getProgress = function () {
    
};

tremppi.tools.addToQueue = function (command) {
    var url = tremppi.getServerAddress() + "?tremppi+" + command;
    $.ajax({
        type: "POST",
        url: url,
        success: function (res) {
            tremppi.log(command + " executed successfully.");
        },
        fail: function (res) {
            tremppi.log(command + " failed.");
        }
    });
};