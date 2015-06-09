/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi */

tremppi.bias.toolbarClick = function (event) {
    if (event.target === 'all') {
        tremppi.report.showAll();
        tremppi.setItem('panel', 'all');
        for (var i = 0; i < tremppi.report.panels.length; i++) {
            tremppi.bias.loadLabels(tremppi.report.panels[i]);
        }
    } else if (event.target === 'left' | event.target === 'mid' | event.target === 'right') {
        tremppi.setItem('panel', event.target);
        tremppi.report.showPanel(event.target);
        tremppi.bias.loadLabels(event.target);
    } else if (event.target.slice(0, 7) === 'select:') {
        var selected = event.target.slice(7);
        tremppi.setItem('selected', selected);
        tremppi.report.pickData(selected, 'left');
    } else if (event.target.slice(0, 8) === 'compare:') {
        var compared = event.target.slice(8);
        tremppi.setItem('compared', compared);
        tremppi.report.pickData(compared, 'right');
    } else if (event.target === 'relative') {
        var checked = !event.item.checked;
        tremppi.setItem('relative', checked);
        for (var i = 0; i < tremppi.report.panels.length; i++) {
            tremppi.bias.applyVisuals(tremppi.report.panels[i]);
            tremppi.bias.loadLabels(tremppi.report.panels[i]);
        }
    }
};