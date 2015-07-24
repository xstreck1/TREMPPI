/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi */

tremppi.correlations.page = function () {
    if (typeof tremppi.correlations.setup === 'undefined') {
        $("#widget").html("No correlations report has been created yet.");
    } else {
        tremppi.report.createPanels();
        for (var i = 0; i < tremppi.report.panels.length; i++) {
            $('#container_' + tremppi.report.panels[i]).html("");
            $("#container_" + tremppi.report.panels[i]).append('<div id="header_' + tremppi.report.panels[i] + '" class="header" >' + tremppi.report.panels[i] + '</div>');
            $("#container_" + tremppi.report.panels[i]).append('<div class="graph" id="graph_'+ tremppi.report.panels[i] + '"></div>');
            $("#container_" + tremppi.report.panels[i]).append('<canvas class="legend" id="legend_'+ tremppi.report.panels[i] + '"></canvas>');
        }
        tremppi.report.initialPanel();
        var setCheck = function (id) {
            var checked = tremppi.getItem(id, tremppi.toolbar.get(id).checked) === "true";
            if (checked) {
                tremppi.toolbar.check(id);
            }
            tremppi.setItem(id, checked);
        };
        setCheck("relative");

        tremppi.correlations.bounds = {
            Pearson: {
                min: -1, max: 1
            },
            Bias: {
                min: 0, max: 1
            }
        };
    }
};

tremppi.correlations.setData = function (data) {
    tremppi.toolbar.onClick = tremppi.correlations.toolbarClick;
};

tremppi.correlations.getData = function () {
    return {};
};

tremppi.correlations.layout = function () {    
    for (var i = 0; i < tremppi.report.panels.length; i++) {
        tremppi.correlations.loadLabels(tremppi.report.panels[i]);
    }
};

tremppi.correlations.toolbarClass = function () {
    return {
        name: 'toolbar',
        items: [
            {type: 'radio', id: 'all', group: '1', caption: 'All', checked: true},
            {type: 'radio', id: 'left', group: '1', caption: 'Left'},
            {type: 'radio', id: 'mid', group: '1', caption: 'Mid'},
            {type: 'radio', id: 'right', group: '1', caption: 'Right'},
            {type: 'break', id: 'break1'},
            {type: 'check', icon: 'w2ui-icon-check', id: 'relative', caption: 'Relative', checked: false}
        ]
    };
};