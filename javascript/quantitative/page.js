/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi */

tremppi.quantitative.page = function () {
    tremppi.toolbar.get('select').items = tremppi.quantitative.setup.files;
    tremppi.toolbar.get('compare').items = tremppi.quantitative.setup.files;
    $("#widget").append('<div id="container_left">left</div>');
    $("#widget").append('<div id="container_mid">mid</div>');
    $("#widget").append('<div id="container_right">right</div>');
    tremppi.quantitative.setPanel('left');
    tremppi.quantitative.setPanel('mid');
    tremppi.quantitative.setPanel('right');
    
    tremppi.quantitative.initialPanel();
    
    tremppi.toolbar.onClick = tremppi.quantitative.toolbarClick;
};



tremppi.quantitative.setData = function (data) {
};

tremppi.quantitative.layout = function () {
};

tremppi.quantitative.toolbarClass = function () {
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