/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi */

tremppi.qualitative.setPage = function () {
    $("#widget").append('<div id="container_left">left</div>');
    $("#widget").append('<div id="container_mid">mid</div>');
    $("#widget").append('<div id="container_right">right</div>');
};

tremppi.qualitative.setPanel = function (data, panel) {
    tremppi.qualitative[panel] = {};
    $.extend(true, tremppi.qualitative[panel], data.grid);
    tremppi.qualitative[panel].name = panel;
    tremppi.qualitative[panel].header = panel;
    tremppi.qualitative[panel] = $('#container_' + panel).w2grid(tremppi.qualitative[panel]);
};

tremppi.qualitative.setData = function (data) {
    tremppi.qualitative.setPanel(data, 'left');
    tremppi.qualitative.setPanel(data, 'mid');
    tremppi.qualitative.setPanel(data, 'right');
    tremppi.qualitative.toolbar.get('select').items = tremppi.data.selections;
    tremppi.qualitative.toolbar.get('compare').items = tremppi.data.selections;
    tremppi.qualitative.toolbar.onClick = tremppi.qualitative.toolbarClick;
    if (data.panel === 'left' || data.panel === 'mid' || data.panel === 'right') {
        tremppi.qualitative.toolbar.uncheck('all');
        tremppi.qualitative.toolbar.check(data.panel);
        tremppi.qualitative.showPanel(data.panel);
    }

    if (data.select !== "") {
        tremppi.qualitative.pickData(data.select, 'left');
    }
    if (data.compare !== "") {
        tremppi.qualitative.pickData(data.compare, 'right');
    }
};

tremppi.qualitative.save = function () {
    tremppi.save();
};

tremppi.qualitative.layout = function () {
};

tremppi.qualitative.setDefaultData = function (data) {
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

tremppi.qualitative.defaultToolbar = function () {
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