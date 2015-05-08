/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi */

tremppi.quantitative.showPanel = function (name) {
    ['left', 'mid', 'right'].forEach(function (panel) {
        if (panel === name) {
            $('#container_' + panel).css('width', '100%').css('display', 'block');
        } else {
            $('#container_' + panel).css('display', 'none');
        }
    });
    tremppi.quantitative[name].resize();
};

tremppi.quantitative.showAll = function () {
    $('#container_left').css('width', '33.33%').css('display', 'block');
    $('#container_mid').css('width', '33.33%').css('display', 'block');
    $('#container_right').css('width', '33.33%').css('display', 'block');
    tremppi.quantitative.left.resize();
    tremppi.quantitative.mid.resize();
    tremppi.quantitative.right.resize();
};

tremppi.quantitative.pickData = function (source, panel) {
    $.getJSON(tremppi.getDataFile(source), tremppi.quantitative.valuesSetter(source, panel));
};

tremppi.quantitative.toolbarClick = function (event) {
    if (event.type === 'click') {
        if (event.target === 'all') {
            tremppi.quantitative.showAll();
            tremppi.data.panel = 'all';
        } else if (event.target === 'left' | event.target === 'mid' | event.target === 'right') {
            tremppi.data.panel = event.target;
            tremppi.quantitative.showPanel(tremppi.data.panel);
        } else if (event.target.slice(0, 7) === 'select:') {
            tremppi.data.select = event.target.slice(7);
            tremppi.quantitative.pickData(tremppi.data.select, 'left');
        } else if (event.target.slice(0, 8) === 'compare:') {
            tremppi.data.compare = event.target.slice(8);
            tremppi.quantitative.pickData(tremppi.data.compare, 'right');
        }
        tremppi.quantitative.save();
    }
    console.log('Clicked: ' + event.target, event);
};