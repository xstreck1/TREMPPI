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

tremppi.regulations.createPanelContent = function (data, panel) {
    tremppi.regulations[panel].load(data.elements);
    $('#header_' + panel).html(data.setup.s_name);
    tremppi.regulations.applyVisuals(panel);
    tremppi.regulations.loadLabels(panel);
    tremppi.regulations.addQtip(panel);
    tremppi.report.setDescription(panel, data.setup);
};

tremppi.regulations.valuesSetter = function (source, panel) {
    return function (data) {
        $('#header_' + panel).html(source);
        tremppi.regulations.createPanelContent(data, panel);
        tremppi.log(source + ' loaded successfully.');
        
        var left_elems = (panel === 'left') ? data.elements : tremppi.regulations.left.json().elements;
        var right_elems = (panel === 'left') ? tremppi.regulations.right.json().elements : data.elements;

        if (typeof left_elems.nodes !== 'undefined' && typeof right_elems.nodes !== 'undefined') {
            $('#header_mid').html($('#header_left').html() + ' - ' + $('#header_right').html());
            var to_synchronize = tremppi.regulations.mid.nodes().length === 0; // Synchronize once

            var mid = {};
            $.extend(true, mid, left_elems);
            // Only make diff if both graphs are loaded
            for (var i = 0; i < right_elems.edges.length; i++) {
                var substract = function (value) {
                    mid.edges[i].data[value] -= right_elems.edges[i].data[value];
                };
                substract('WeightedFrequency');
                substract('Frequency');
                substract('Pearson');
            }
            var mid_data = {'elements': mid, 'setup': {'s_name' : $('#header_left').html() + ' - ' + $('#header_right').html()}};
            tremppi.regulations.createPanelContent(mid_data, 'mid');

            if (to_synchronize) {
                tremppi.cytoscape.synchronize(tremppi.regulations.loadLabels);
            }
        }
    };
};

tremppi.regulations.setPanel = function (panel) {
    var selected_col = '#5555BB';
    tremppi.regulations[panel] = cytoscape({
        container: document.getElementById('graph_' + panel),
        style: cytoscape.stylesheet()
                .selector('node')
                .css({
                    'content': 'data(name)',
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
                    'content': 'data(Threshold)',
                    'text-outline-color': 'black',
                    'text-outline-width': 1,
                    'color': 'white',
                    'font-size': 25
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
    tremppi.regulations[panel].on('zoom', function() { tremppi.regulations.loadLabels(panel); });
};

// Change between relative and absolute values
tremppi.regulations.applyVisuals = function (panel) {
    var relative = tremppi.getItem('relative') === 'true';
    var weighted = tremppi.getItem('weighted') === 'true';

    var width_param = weighted ? 'WeightedFrequency' : 'Frequency';
    var range = tremppi.report.getRange(panel, relative, 'edge[Frequency]', width_param);
    var abs_max =  Math.max(Math.abs(range.min), Math.abs(range.max));
    tremppi.cytoscape.mapRange(panel, 'edge[Frequency>=0]', width_param, 'width', 0, abs_max, 1, 10);
    tremppi.cytoscape.mapRange(panel, 'edge[Frequency<0]', width_param, 'width', -1*abs_max, 0, 10, 1);

    range = tremppi.report.getRange(panel, relative, 'edge[Pearson]', 'Pearson');
    tremppi.cytoscape.mapRange(panel, 'edge[Pearson>=0]', 'Pearson', 'line-color', 0, range.max, 'yellow', 'green');
    tremppi.cytoscape.mapRange(panel, 'edge[Pearson<0]', 'Pearson', 'line-color', range.min, 0, 'red', 'yellow');

    tremppi.cytoscape.mapValue(panel, 'edge[Sign="0"]', 'target-arrow-shape', 'circle');
    tremppi.cytoscape.mapValue(panel, 'edge[Sign="+"]', 'target-arrow-shape', 'triangle');
    tremppi.cytoscape.mapValue(panel, 'edge[Sign="-"]', 'target-arrow-shape', 'tee');
    tremppi.cytoscape.mapValue(panel, 'edge[Sign="1"]', 'target-arrow-shape', 'triangle-tee');

    tremppi.cytoscape.mapValue(panel, 'edge[Frequency>0]', 'line-style', 'solid');
    tremppi.cytoscape.mapValue(panel, 'edge[Frequency=0]', 'line-style', 'dotted');
    tremppi.cytoscape.mapValue(panel, 'edge[Frequency<0]', 'line-style', 'dashed');
};

// Adds reactive tip window that appears on mouseover on the edge
tremppi.regulations.addQtip = function (panel) {
    var num_of_decimals = 3;

    var edges = tremppi.regulations[panel].edges();
    var labeller = function (my_data) {
        return 'source: ' + my_data.source + '<br />'
                + 'Threshold: ' + my_data.Threshold + '<br />'
                + 'target: ' + my_data.target + '<br />'
                + 'Impact: ' + my_data.Pearson.toFixed(num_of_decimals) + '<br />'
                + 'Frequency: ' + my_data.Frequency.toFixed(num_of_decimals) + '<br />'
                //+ 'Weighted Frequency: ' + my_data.WeightedFrequency.toFixed(num_of_decimals) + '<br />'
                ;
    };

    tremppi.qtip.addOnHoverLabeller(panel, edges, labeller);
};

tremppi.regulations.bar_left = 110;
tremppi.regulations.num_of_decimals = 3;
tremppi.regulations.I_height = 20;
tremppi.regulations.F_height = 40;

tremppi.regulations.loadLabels = function (panel) {
    var graph = tremppi.regulations[panel];

    var relative = tremppi.getItem('relative') === 'true';
    var weighted = tremppi.getItem('weighted') === 'true' ? 'WeightedFrequency' : 'Frequency';

    var my_paper = new paper.PaperScope();

    var legend_height = Math.min(10 * graph.zoom() + 35, $('#container_' + panel).height() / 3);
    $('#legend_' + panel).height(legend_height);
    my_paper.setup($('#legend_' + panel)[0]);

    my_paper.activate();
    tremppi.regulations.addGradient(relative, panel, my_paper);
    tremppi.regulations.addEdgeWidth(relative, weighted, panel, my_paper, graph.zoom());
    my_paper.view.draw();
};

tremppi.regulations.addGradient = function (relative, panel, my_paper) {
    var range = tremppi.report.getRange(panel, relative, 'edge', 'Pearson');
    var bar_right = my_paper.view.viewSize.width - 80;

    // Make the bar   
    var bar = new paper.Path.Rectangle(
            new paper.Rectangle(
                    new paper.Point(tremppi.regulations.bar_left, tremppi.regulations.I_height - 10),
                    new paper.Point(bar_right, tremppi.regulations.I_height)));
    bar.fillColor = {gradient: {}};
    bar.fillColor.origin = [tremppi.regulations.bar_left, 0];
    bar.fillColor.destination = [bar_right, 0];
    bar.fillColor.gradient.stops = ['red', 'yellow', 'green'];
    bar.strokeColor = 'black';
    bar.strokeWidth = 1;
    // Make the text
    tremppi.paper.makeText('I: ', new paper.Point(10, tremppi.regulations.I_height));
    tremppi.paper.makeText(
            range.min.toFixed(tremppi.regulations.num_of_decimals),
            new paper.Point(tremppi.regulations.bar_left - 75, tremppi.regulations.I_height)
            );
    tremppi.paper.makeText(
            '0',
            new paper.Point(my_paper.view.viewSize.width/2, tremppi.regulations.I_height)
            );
    tremppi.paper.makeText(
            range.max.toFixed(tremppi.regulations.num_of_decimals),
            new paper.Point(bar_right + 5, tremppi.regulations.I_height)
            );
};

tremppi.regulations.addEdgeWidth = function (relative, weighted, panel, my_paper, width_ratio) {
    var range = tremppi.report.getRange(panel, relative, 'edge', weighted);
    var abs_max = Math.max(Math.abs(range.min), Math.abs(range.max));
    var bar_height = tremppi.regulations.F_height - 10;
    var bar_right = my_paper.view.viewSize.width - 80;

    // Create the bar
    var bar = new paper.Path();
    bar.fillColor = 'black';
    bar.strokeWidth = 0;
    var min_width = 1 * width_ratio;
    var max_width = 10 * width_ratio;
    bar.add(new paper.Point(bar_right, bar_height));
    bar.add(new paper.Point(tremppi.regulations.bar_left, bar_height + (max_width - min_width) / 2));
    bar.add(new paper.Point(tremppi.regulations.bar_left, bar_height + (max_width + min_width) / 2));
    bar.add(new paper.Point(bar_right, bar_height + max_width));
    bar.add(new paper.Point(bar_right, bar_height));
    // Add the label
    var F_pad = max_width / 2 - 4;
    tremppi.paper.makeText('F: ', new paper.Point(10, tremppi.regulations.F_height + F_pad));
    tremppi.paper.makeText(
            (0.0).toFixed(tremppi.regulations.num_of_decimals),
            new paper.Point(tremppi.regulations.bar_left - 75, tremppi.regulations.F_height + F_pad));
    tremppi.paper.makeText(
            abs_max.toFixed(tremppi.regulations.num_of_decimals),
            new paper.Point(bar_right + 5, tremppi.regulations.F_height + F_pad));
};
