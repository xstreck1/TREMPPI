/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi */

tremppi.summary.setPage = function () {
    $("#widget").append('<div id="container_left">left</div>');
    $("#widget").append('<div id="container_mid">mid</div>');
    $("#widget").append('<div id="container_right">right</div>');
};

tremppi.summary.setPanel = function (data, panel) {
    tremppi.summary[panel] = {};
    $.extend(true, tremppi.summary[panel], data.grid);
    tremppi.summary[panel].name = panel;
    tremppi.summary[panel].header = panel;
    tremppi.summary[panel] = $('#container_' + panel).w2grid(tremppi.summary[panel]);
};

tremppi.summary.setData = function (data) {
    tremppi.summary.setPanel(data, 'left');
    tremppi.summary.setPanel(data, 'mid');
    tremppi.summary.setPanel(data, 'right');
    tremppi.summary.toolbar.get('select').items = tremppi.data.selections;
    tremppi.summary.toolbar.get('compare').items = tremppi.data.selections;
    tremppi.summary.toolbar.onClick = tremppi.summary.toolbarClick;
    if (data.panel === 'left' || data.panel === 'mid' || data.panel === 'right') {
        tremppi.summary.toolbar.uncheck('all');
        tremppi.summary.toolbar.check(data.panel);
        tremppi.summary.showPanel(data.panel);
    }

    if (data.select !== "") {
        tremppi.summary.pickData(data.select, 'left');
    }
    if (data.compare !== "") {
        tremppi.summary.pickData(data.compare, 'right');
    }
};

tremppi.summary.save = function () {
    tremppi.save();
};

tremppi.summary.layout = function () {
};

tremppi.summary.setDefaultData = function (data) {
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
    var portion = (100 - 50) / 5;
    data.grid.columns = [
        {field: 'name', caption: 'Name', size: '50%', sortable: true},
        {field: 'count', caption: 'Count', size: portion + '%', sortable: true},
        {field: 'portion', caption: 'Portion', size: portion + '%', sortable: true},
        {field: 'min', caption: 'Min', size: portion + '%', sortable: true},
        {field: 'max', caption: 'Max', size: portion + '%', sortable: true},
        {field: 'mean', caption: 'Mean', size: portion + '%', sortable: true}
    ];
    data.grid.records = [];
};

tremppi.summary.defaultToolbar = function () {
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