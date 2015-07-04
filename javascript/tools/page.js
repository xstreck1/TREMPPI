/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi */

tremppi.tools.tool_list = ['spawn', 'validate'];

tremppi.tools.page = function () {
    $('#widget').append('<div id="commands"></div>');
//    for (var i = 0; i < tremppi.tools.tool_list.length; i++) {
//        var tool_name = tremppi.tools.tool_list[i];
//        $('#commands').append('<button id"' + tool_name + '">' + tool_name + '</button>');
//    }
    $('#widget').append('<table id="running_process"></table>');
};

tremppi.tools.layout = function () {
};

tremppi.tools.setData = function (data) {
    // Enable controls
    tremppi.tools.controls();
};

tremppi.tools.toolbarClass = function () {
    return {
        name: 'toolbar',
        items: [
            {type: 'button', id: 'spawn', caption: 'Enumerate', img: 'icon-page', hint: 'Enumerate the parametrizations based on the network.'},
            {type: 'button', id: 'bias', caption: 'Bias', img: 'icon-page', hint: 'Enumerate the parametrizations based on the network.'},
            {type: 'button', id: 'sign', caption: 'Sign', img: 'icon-page', hint: 'Add edge signs to all known regulations.'}
        ]
    };
};

tremppi.tools.save = function () {
};
