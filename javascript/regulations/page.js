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
            {type: 'check', icon: 'w2ui-icon-check', id: 'relative', caption: 'Relative', checked: false}
            // {type: 'check', icon: 'w2ui-icon-check', id: 'weighted', caption: 'Weighted', checked: false}
        ]
    };
};

tremppi.regulations.compareData = function (A, B) {
    return true;
};