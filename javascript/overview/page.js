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
    $.extend(true, tremppi.overview.left, data.grid);
    tremppi.overview.left.name = 'left';
    tremppi.overview.left.header = 'left';
    $.extend(true, tremppi.overview.mid, data.grid);
    tremppi.overview.mid.name = 'mid';
    tremppi.overview.mid.header = 'mid';
    $.extend(true, tremppi.overview.right, data.grid);
    tremppi.overview.right.name = 'right';
    tremppi.overview.right.header = 'right';
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


    data.grid = {};
    data.grid.name = 'grid';
    data.grid.header = "";
    data.grid.show = {
        header: true
    };
    data.grid.columns = [
        {field: 'count', caption: 'Count', size: '20%'},
        {field: 'portion', caption: 'Portion', size: '20%'},
        {field: 'min', caption: 'Min', size: '20%'},
        {field: 'max', caption: 'Max', size: '20%'},
        {field: 'mean', caption: 'Mean', size: '20%'}
    ];
    data.grid.records = [];
};

tremppi.overview.defaultToolbar = function () {
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