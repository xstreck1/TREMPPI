/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi */

tremppi.occurence.setPage = function () {
    $("#widget").append('<div id="container_left">left</div>');
    $("#widget").append('<div id="container_mid">mid</div>');
    $("#widget").append('<div id="container_right">right</div>');
};

tremppi.occurence.setPanel = function (data, panel) {
    tremppi.occurence[panel] = {};
    $.extend(true, tremppi.occurence[panel], data.grid);
    tremppi.occurence[panel].name = panel;
    tremppi.occurence[panel].header = panel;
    tremppi.occurence[panel] = $('#container_' + panel).w2grid(tremppi.occurence[panel]);
};

tremppi.occurence.setData = function (data) {
    tremppi.occurence.setPanel(data, 'left');
    tremppi.occurence.setPanel(data, 'mid');
    tremppi.occurence.setPanel(data, 'right');
    tremppi.occurence.toolbar.get('select').items = tremppi.data.selections;
    tremppi.occurence.toolbar.get('compare').items = tremppi.data.selections;
    tremppi.occurence.toolbar.onClick = tremppi.occurence.toolbarClick;
    if (data.panel === 'left' || data.panel === 'mid' || data.panel === 'right') {
        tremppi.occurence.toolbar.uncheck('all');
        tremppi.occurence.toolbar.check(data.panel);
        tremppi.occurence.showPanel(data.panel);
    }

    if (data.select !== "") {
        tremppi.occurence.pickData(data.select, 'left');
    }
    if (data.compare !== "") {
        tremppi.occurence.pickData(data.compare, 'right');
    }
};

tremppi.occurence.save = function () {
    tremppi.save();
};

tremppi.occurence.layout = function () {
};

tremppi.occurence.setDefaultData = function (data) {
    if (typeof data.selections === 'undefined')
        data.selections = [];

    if (typeof data.select === 'undefined')
        data.select = "";
    if (typeof data.compare === 'undefined')
        data.compare = "";
    if (typeof data.panel === 'undefined')
        data.panel = "";

    data.grid = {};
    data.grid.name = 'grid';
    data.grid.header = "";
    data.grid.show = {
        header: true
    };
    data.grid.columns = [
        {field: 'name', caption: 'Name', size: '20%', sortable: true},
        {field: 'value', caption: 'Value', size: '80%'}
    ];
    data.grid.records = [];
};

tremppi.occurence.defaultToolbar = function () {
    return {
        name: 'toolbar',
        items: [
            {type: 'menu', id: 'select', caption: 'Select', items: []},
            {type: 'menu', id: 'compare', caption: 'Compare', items: []},
            {type: 'break', id: 'break0'},
            {type: 'radio', id: 'all', group: '1', caption: 'All', checked: true},
            {type: 'radio', id: 'left', group: '1', caption: 'Left'},
            {type: 'radio', id: 'mid', group: '1', caption: 'Mid'},
            {type: 'radio', id: 'right', group: '1', caption: 'Right'}
        ]
    };
};