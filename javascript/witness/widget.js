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

/* global tremppi, cytoscape, paper */

tremppi.witness.createPanelContent = function (data, panel) {
    tremppi.witness[panel].load(data.elements);
    $('#header_' + panel).html(data.setup.s_name);
    tremppi.witness.applyVisuals(panel);
    tremppi.witness.loadLabels(panel);
    tremppi.witness.addQtip(panel);
    tremppi.report.setDescription(panel, data.setup);
};

tremppi.witness.setPanel = function (panel) {
    var selected_col = '#5555BB';
    tremppi.widget[panel] = cytoscape({
        container: document.getElementById('graph_' + panel),
        style: cytoscape.stylesheet()
                .selector('node')
                .css({
                    'content': 'data(Name)',
                    'text-valign': 'center',
                    'background-color': 'white',
                    'border-color': 'black',
                    'border-width': 1,
                    'shape': 'rectangle',
                    'width': 100,
                    'height': 25,
                    'font-size': 15
                })
                .selector('edge')
                .css({
                    'target-arrow-color': 'black',
                    'content': 'data(Step)',
                    'text-outline-color': 'black',
                    'text-outline-width': 2,
                    'color': 'white',
                    'font-size': 20,
                    'target-arrow-shape': 'triangle'
                }).selector(':selected')
                .css({
                    'border-color': selected_col,
                    'line-color': selected_col,
                    'target-arrow-color': selected_col
                }),
        layout: {
            name: 'grid'
        },
        wheelSensitivity: 0.2
    });
    
    tremppi.witness[panel].on('zoom', function() { tremppi.witness.loadLabels(panel); });
};


tremppi.witness.valuesSetter = function (source, panel) {
    return function (data) {
        if (typeof data.elements === 'undefined' || typeof data.elements.nodes === 'undefined' || typeof data.elements.edges === 'undefined') {
            tremppi.log('The witness file does not contain a graph.', 'warning');
            return;
        }

        tremppi.witness.createPanelContent(data, panel);
        tremppi.log(source + ' loaded successfully.');
        
        var left_elems = (panel === 'left') ? data.elements : tremppi.witness.left.json().elements;
        var right_elems = (panel === 'left') ? tremppi.witness.right.json().elements : data.elements;

        if (typeof left_elems.nodes !== 'undefined' && typeof right_elems.nodes !== 'undefined') {
            $('#header_mid').html($('#header_left').html() + ' - ' + $('#header_right').html());
            var to_synchronize = tremppi.witness.mid.nodes().length === 0;

            var mid = {};
            $.extend(true, mid, left_elems);
            
            // Add the edges from left not found in the right
            // Is quadratic, could be improved
            for (var i = 0; i < mid.edges.length; i++) {
                for (var j = 0; j < right_elems.edges.length; j++) {
                    if (mid.edges[i].data.Step === right_elems.edges[j].data.Step &&
                            mid.edges[i].data.source === right_elems.edges[j].data.source &&
                            mid.edges[i].data.target === right_elems.edges[j].data.target)
                    {
                        mid.edges[i].data.Frequency -= right_elems.edges[j].data.Frequency;
                    }
                }
            }

            var mid_data = {'elements': mid, 'setup': {'s_name' : $('#header_left').html() + ' - ' + $('#header_right').html()}};
            tremppi.witness.createPanelContent(mid_data, 'mid');

            if (to_synchronize) {
                tremppi.cytoscape.synchronize(tremppi.witness.loadLabels);
            }
        }
    };
};

// Change between relative and absolute values
tremppi.witness.applyVisuals = function (panel) {
    var relative = tremppi.getItem('relative') === 'true';

    var range = tremppi.report.getRange(panel, relative, 'edge', "Frequency");
    var abs_max =  Math.max(Math.abs(range.min), Math.abs(range.max));
    tremppi.cytoscape.mapRange(panel, 'edge[Frequency>=0]', "Frequency", 'width', 0, abs_max, 1, 10);
    tremppi.cytoscape.mapRange(panel, 'edge[Frequency<0]', "Frequency", 'width', -1*abs_max, 0, 10, 1);

    tremppi.cytoscape.mapValue(panel, 'edge[Frequency>0]', 'line-style', 'solid');
    tremppi.cytoscape.mapValue(panel, 'edge[Frequency=0]', 'line-style', 'dotted');
    tremppi.cytoscape.mapValue(panel, 'edge[Frequency<0]', 'line-style', 'dashed');
};


tremppi.witness.addQtip = function(panel) {
    var elements = tremppi.witness[panel].elements();
    var labeller = function (my_data) {
        if (typeof my_data.Frequency !== 'undefined') {
            return 'source: ' + my_data.source + '<br />'
                    + 'target: ' + my_data.target + '<br />'
                    + 'Frequency: ' + my_data.Frequency.toFixed(tremppi.witness.num_of_decimals) + '<br />';
        } 
        else {
            var comps = "";
            for (var i = 0; i < tremppi.witness.setup.components.length; i++) {
                comps += tremppi.witness.setup.components[i] + ": " + my_data.id[i] + '<br />';
            }
            comps += 'BA state: ' + my_data.id[i] + '<br />';
            return comps;
         }
    };
    tremppi.qtip.addOnHoverLabeller(panel, elements, labeller);
};

tremppi.witness.bar_left = 110;
tremppi.witness.num_of_decimals = 3;
tremppi.witness.F_height = 20;

tremppi.witness.loadLabels = function(panel) {
    var graph = tremppi.witness[panel];

    var relative = tremppi.getItem('relative') === 'true';

    var my_paper = new paper.PaperScope();

    var legend_height = Math.min(10 * graph.zoom() + 15, $('#container_' + panel).height() / 3);
    $('#legend_' + panel).height(legend_height);
    my_paper.setup($('#legend_' + panel)[0]);

    my_paper.activate();
    tremppi.witness.addEdgeWidth(relative, panel, my_paper, graph.zoom());
    my_paper.view.draw();
};

tremppi.witness.addEdgeWidth = function (relative, panel, my_paper, width_ratio) {
    var range = tremppi.report.getRange(panel, relative, 'edge', 'Frequency');
    var abs_max = Math.max(Math.abs(range.min), Math.abs(range.max));
    var bar_height = tremppi.witness.F_height - 10;
    var bar_right = my_paper.view.viewSize.width - 80;

    // Create the bar
    var bar = new paper.Path();
    bar.fillColor = 'black';
    bar.strokeWidth = 0;
    var min_width = 1 * width_ratio;
    var max_width = 10 * width_ratio;
    bar.add(new paper.Point(bar_right, bar_height));
    bar.add(new paper.Point(tremppi.witness.bar_left, bar_height + (max_width - min_width) / 2));
    bar.add(new paper.Point(tremppi.witness.bar_left, bar_height + (max_width + min_width) / 2));
    bar.add(new paper.Point(bar_right, bar_height + max_width));
    bar.add(new paper.Point(bar_right, bar_height));
    // Add the label
    var F_pad = max_width / 2 - 4;
    tremppi.paper.makeText('F: ', new paper.Point(10, tremppi.witness.F_height + F_pad));
    tremppi.paper.makeText(
            (0.0).toFixed(tremppi.witness.num_of_decimals),
            new paper.Point(tremppi.witness.bar_left - 75, tremppi.witness.F_height + F_pad));
    tremppi.paper.makeText(
            abs_max.toFixed(tremppi.witness.num_of_decimals),
            new paper.Point(bar_right + 5, tremppi.witness.F_height + F_pad));
};
