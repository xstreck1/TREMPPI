/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi */

tremppi.witness.page = function () {
    if (typeof tremppi.witness.setup === 'undefined') {
        $("#widget").html("No witness has been created yet.");
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
        
        tremppi.witness.bounds = {
            Frequency: {
                min: 0, max: 1
            }
        };
        
    }
};

tremppi.witness.setData = function (data) {
    tremppi.toolbar.onClick = tremppi.witness.toolbarClick;
};

tremppi.witness.getData = function () {
    return {};
};

tremppi.witness.layout = function () {
    for (var i = 0; i < tremppi.report.panels.length; i++) {
        tremppi.witness.loadLabels(tremppi.report.panels[i]);
    }
};

tremppi.witness.toolbarClass = function () {
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