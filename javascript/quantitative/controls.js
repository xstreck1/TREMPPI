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
    tremppi.getData(tremppi.quantitative.valuesSetter(source, panel), source);
};

tremppi.quantitative.toolbarClick = function (event) {
    if (event.target === 'all') {
        tremppi.quantitative.showAll();
        tremppi.setItem('panel', 'all');
    } else if (event.target === 'left' | event.target === 'mid' | event.target === 'right') {
        tremppi.setItem('panel', event.target);
        tremppi.quantitative.showPanel(event.target);
    } else if (event.target.slice(0, 7) === 'select:') {
        var selected = event.target.slice(7);
        tremppi.setItem('selected', selected);
        tremppi.quantitative.pickData(selected, 'left');
    } else if (event.target.slice(0, 8) === 'compare:') {
        var compared = event.target.slice(8);
        tremppi.setItem('compared', selected);
        tremppi.quantitative.pickData(compared, 'right');
    }
};