/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi */

tremppi.regulations.null_pearson = "yellow";

tremppi.regulations.page = function () {
    if (typeof tremppi.regulations.setup === 'undefined') {
        $("#widget").html("No regulations report has been created yet.");
    } else {
        tremppi.report.createPanels();
        for (var i = 0; i < tremppi.report.panels.length; i++) {
            $('#container_' + tremppi.report.panels[i]).html("");
            $("#container_" + tremppi.report.panels[i]).append('<div id="header_' + tremppi.report.panels[i] + '" class="header" >' + tremppi.report.panels[i] + '</div>');
            $("#container_" + tremppi.report.panels[i]).append('<div class="report_content" id="graph_' + tremppi.report.panels[i] + '"></div>');
            $("#container_" + tremppi.report.panels[i]).append('<canvas class="legend" id="legend_' + tremppi.report.panels[i] + '"></canvas>');
            $("#container_" + tremppi.report.panels[i]).append('<div id="desc_' + tremppi.report.panels[i] + '" class="description" ></div>');
        }
        tremppi.report.initialPanel();
        tremppi.common.setCheck("relative");
        // tremppi.common.setCheck("weighted");

        tremppi.regulations.bounds = {
            Pearson: {
                min: -1, max: 1
            },
            Frequency: {
                min: 0, max: 1
            },
            WeightedFrequency: {
                min: 0, max: 1
            }
        };
    }
};

tremppi.regulations.setData = function (data) {
    tremppi.toolbar.onClick = tremppi.regulations.toolbarClick;
};

tremppi.regulations.getData = function () {
    return {};
};

tremppi.regulations.layout = function () {
    for (var i = 0; i < tremppi.report.panels.length; i++) {
        tremppi.regulations.loadLabels(tremppi.report.panels[i]);
    }
};

tremppi.regulations.toolbarClass = function () {
    return {
        name: 'toolbar',
        items: [
            {type: 'radio', id: 'all', group: '1', caption: 'All', checked: true},
            {type: 'radio', id: 'left', group: '1', caption: 'Left'},
            {type: 'radio', id: 'mid', group: '1', caption: 'Mid'},
            {type: 'radio', id: 'right', group: '1', caption: 'Right'},
            {type: 'break', id: 'break1'},
            {type: 'check', icon: 'w2ui-icon-check', id: 'relative', caption: 'Relative', checked: false},
            // {type: 'check', icon: 'w2ui-icon-check', id: 'weighted', caption: 'Weighted', checked: false}
        ]
    };
};