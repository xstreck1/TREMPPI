/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi */

tremppi.qualitative.page = function () {
    if (typeof tremppi.qualitative.setup === 'undefined') {
        $("#widget").html("No qualitative report has been created yet.");
    } else {
        tremppi.report.createPanels();
        tremppi.report.initialPanel();
        tremppi.toolbar.onClick = tremppi.qualitative.toolbarClick;
    }
};

tremppi.qualitative.setData = function (data) {
};

tremppi.qualitative.layout = function () {
};

tremppi.qualitative.toolbarClass = function () {
    return {
        name: 'toolbar',
        items: [{type: 'menu', id: 'select', caption: 'Select', items: []},
            {type: 'menu', id: 'compare', caption: 'Compare', items: []},
            {type: 'break', id: 'break0'},
            {type: 'radio', id: 'all', group: '1', caption: 'All', checked: true},
            {type: 'radio', id: 'left', group: '1', caption: 'Left'},
            {type: 'radio', id: 'mid', group: '1', caption: 'Mid'},
            {type: 'radio', id: 'right', group: '1', caption: 'Right'}]
    };
};
