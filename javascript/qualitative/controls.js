/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi */

tremppi.qualitative.showPanel = function (name) {
    ['left', 'mid', 'right'].forEach(function (panel) {
        if (panel === name) {
            $('#container_' + panel).css('width', '100%').css('display', 'block');
        } else {
            $('#container_' + panel).css('display', 'none');
        }
    });
    tremppi.qualitative[name].resize();
};

tremppi.qualitative.showAll = function () {
    $('#container_left').css('width', '33.33%').css('display', 'block');
    $('#container_mid').css('width', '33.33%').css('display', 'block');
    $('#container_right').css('width', '33.33%').css('display', 'block');
    tremppi.qualitative.left.resize();
    tremppi.qualitative.mid.resize();
    tremppi.qualitative.right.resize();
};

tremppi.qualitative.pickData = function (source, panel) {
    $.getJSON(tremppi.getDataFile(source), tremppi.qualitative.valuesSetter(source, panel));
};

tremppi.qualitative.toolbarClick = function (event) {
    if (event.type === 'click') {
        if (event.target === 'all') {
            tremppi.qualitative.showAll();
            tremppi.data.panel = 'all';
        } else if (event.target === 'left' | event.target === 'mid' | event.target === 'right') {
            tremppi.data.panel = event.target;
            tremppi.qualitative.showPanel(tremppi.data.panel);
        } else if (event.target.slice(0, 7) === 'select:') {
            tremppi.data.select = event.target.slice(7);
            tremppi.qualitative.pickData(tremppi.data.select, 'left');
        } else if (event.target.slice(0, 8) === 'compare:') {
            tremppi.data.compare = event.target.slice(8);
            tremppi.qualitative.pickData(tremppi.data.compare, 'right');
        }
        tremppi.qualitative.save();
    }
    console.log('Clicked: ' + event.target, event);
};