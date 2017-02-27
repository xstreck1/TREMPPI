/******************************************************************************
Created by Adam Streck, 2013-2015, adam.streck@fu-berlin.de

This file is part of the Toolkit for Reverse Engineering of Molecular Pathways
via Parameter Identification (TREMPPI)

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

/* global tremppi */

tremppi.correlations.page = function () {
    if (typeof tremppi.correlations.setup === 'undefined') {
        $("#widget").html("No correlations report has been created yet.");
    } else {
        tremppi.report.createPanels();
        for (var i = 0; i < tremppi.report.panels.length; i++) {
            $('#container_' + tremppi.report.panels[i]).html("");
            $("#container_" + tremppi.report.panels[i]).append('<div id="header_' + tremppi.report.panels[i] + '" class="header" >' + tremppi.report.panels[i] + '</div>');
            $("#container_" + tremppi.report.panels[i]).append('<div class="report_content" id="graph_'+ tremppi.report.panels[i] + '"></div>');
            $("#container_" + tremppi.report.panels[i]).append('<canvas class="legend" id="legend_'+ tremppi.report.panels[i] + '"></canvas>');
            $("#container_" + tremppi.report.panels[i]).append('<div id="desc_' + tremppi.report.panels[i] + '" class="description" ></div>');
        }
        tremppi.report.initialPanel();
        tremppi.common.setCheck("relative");

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
            {type: 'check', icon: 'w2ui-icon-check', id: 'relative', caption: 'Relative', checked: false}
        ]
    };
};

tremppi.correlations.compareData = function (A, B) {
    return true;
};