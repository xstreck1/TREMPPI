/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.overview.setPage = function () {
    $("#widget").append('<div id="container_left">left</div>');
    $("#widget").append('<div id="container_mid">mid</div>');
    $("#widget").append('<div id="container_right">right</div>');
};

tremppi.overview.setData = function (data) {
    tremppi.overview.left = {}, tremppi.overview.mid = {}, tremppi.overview.right = {};
    $.extend(true, tremppi.overview.left, data);
    tremppi.overview.left.name = 'left';
    $.extend(true, tremppi.overview.mid, data);
    tremppi.overview.mid.name = 'mid';
    $.extend(true, tremppi.overview.right, data);
    tremppi.overview.right.name = 'right';
    tremppi.overview.left = $('#container_left').w2grid(tremppi.overview.left);
    tremppi.overview.mid = $('#container_mid').w2grid(tremppi.overview.mid);
    tremppi.overview.right = $('#container_right').w2grid(tremppi.overview.right);

    tremppi.overview.toolbar.get('select').items = tremppi.data.selections;
    tremppi.overview.toolbar.get('compare').items = tremppi.data.selections;
    tremppi.overview.toolbar.onClick = tremppi.overview.toolbarClick;
};

tremppi.overview.save = function () {
    tremppi.save();
};

tremppi.overview.layout = function () {
};

tremppi.overview.setDefaultData = function (data) {
    if (typeof data.selections === 'undefined')
        data.selections = [];

    if (typeof data.left === 'undefined')
        data.left = "";
    if (typeof data.right === 'undefined')
        data.right = "";

    if (typeof data.grid === 'undefined')
        data.grid = {};

    if (typeof data.grid.name === 'undefined')
        data.grid.name = 'grid';
    if (typeof data.grid.columns === 'undefined')
        data.grid.columns = [];
    if (typeof data.grid.records === 'undefined')
        data.grid.records = [];
};

tremppi.overview.defaultToolbar = function () {
    return {
        name: 'toolbar',
        items: [
            {type: 'menu', id: 'select', caption: 'Select', items: []},
            {type: 'menu', id: 'compare', caption: 'Compare', items: []},
            { type: 'break',  id: 'break0' },
            { type: 'radio',  id: 'all',  group: '1', caption: 'All', checked: true  },
            { type: 'radio',  id: 'left',  group: '1', caption: 'Left' },
            { type: 'radio',  id: 'mid',  group: '1', caption: 'Mid' },
            { type: 'radio',  id: 'right',  group: '1', caption: 'Right' }
        ]
    };
};