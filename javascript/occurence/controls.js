/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi */

tremppi.occurence.showPanel = function (name) {
    ['left', 'mid', 'right'].forEach(function (panel) {
        if (panel === name) {
            $('#container_' + panel).css('width', '100%').css('display', 'block');
        } else {
            $('#container_' + panel).css('display', 'none');
        }
    });
    tremppi.occurence[name].resize();
};

tremppi.occurence.showAll = function () {
    $('#container_left').css('width', '33.33%').css('display', 'block');
    $('#container_mid').css('width', '33.33%').css('display', 'block');
    $('#container_right').css('width', '33.33%').css('display', 'block');
    tremppi.occurence.left.resize();
    tremppi.occurence.mid.resize();
    tremppi.occurence.right.resize();
};

tremppi.occurence.pickData = function (source, panel) {
    $.getJSON(tremppi.getDataFile(source), tremppi.occurence.valuesSetter(source, panel));
};

tremppi.occurence.toolbarClick = function (event) {
    if (event.type === 'click') {
        if (event.target === 'all') {
            tremppi.occurence.showAll();
            tremppi.data.panel = 'all';
        } else if (event.target === 'left' | event.target === 'mid' | event.target === 'right') {
            tremppi.data.panel = event.target;
            tremppi.occurence.showPanel(tremppi.data.panel);
        } else if (event.target.slice(0, 7) === 'select:') {
            tremppi.data.select = event.target.slice(7);
            tremppi.occurence.pickData(tremppi.data.select, 'left');
        } else if (event.target.slice(0, 8) === 'compare:') {
            tremppi.data.compare = event.target.slice(8);
            tremppi.occurence.pickData(tremppi.data.compare, 'right');
        }
        tremppi.occurence.save();
    }
    console.log('Clicked: ' + event.target, event);
};