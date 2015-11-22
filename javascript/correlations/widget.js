/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi, cytoscape, paper */

tremppi.correlations.createPanelContent = function (data, panel) {
    tremppi.correlations[panel].load(data.elements);
    $('#header_' + panel).html(data.setup.s_name);
    tremppi.correlations.applyVisuals(panel);
    tremppi.correlations.loadLabels(panel);
    tremppi.correlations.addQtip(panel);
    tremppi.report.setDescription(panel, data.setup);
};

tremppi.correlations.valuesSetter = function (source, panel) {
    return function (data) {
        tremppi.correlations.createPanelContent(data, panel);
        tremppi.log(source + ' loaded successfully.');

        var left_elems = (panel === 'left') ? data.elements : tremppi.correlations.left.json().elements;
        var right_elems = (panel === 'left') ? tremppi.correlations.right.json().elements : data.elements;

        if (tremppi.correlations.left.nodes().length > 0 && tremppi.correlations.right.nodes().length > 0) {
            $('#header_mid').html();
            var to_synchronize = tremppi.correlations.mid.nodes().length === 0; // Synchronize once

            var mid = {};
            $.extend(true, mid, left_elems);
            for (var i = 0; i < right_elems.edges.length; i++) {
                mid.edges[i].data['Pearson'] -= right_elems.edges[i].data['Pearson'];
            }
            for (var i = 0; i < right_elems.nodes.length; i++) {
                mid.nodes[i].data['Bias'] -= right_elems.nodes[i].data['Bias'];
            }

            var mid_data = {'elements': mid, 'setup': {'s_name' : $('#header_left').html() + ' - ' + $('#header_right').html()}};
            tremppi.correlations.createPanelContent(mid_data, 'mid');

            if (to_synchronize) {
                tremppi.cytoscape.synchronize(tremppi.correlations.loadLabels);
            }
        }
    };
};

tremppi.correlations.setPanel = function (panel) {
    var selected_col = '#5555BB';
    tremppi.correlations[panel] = cytoscape({
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
                    'width': 5
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
    tremppi.correlations[panel].on('zoom', function() { tremppi.correlations.loadLabels(panel); });
};

// Change between relative and absolute values
tremppi.correlations.applyVisuals = function (panel) {
    var relative = tremppi.getItem('relative') === 'true';

    var range = tremppi.report.getRange(panel, relative, 'node', 'Bias');
    var abs_max =  Math.max(Math.abs(range.min), Math.abs(range.max));
    tremppi.cytoscape.mapRange(panel, 'node[Bias>=0]', 'Bias', 'border-width', 0, abs_max, 1, 10);
    tremppi.cytoscape.mapRange(panel, 'node[Bias<0]', 'Bias', 'border-width', -1*abs_max, 0, 10, 1);

    var range = tremppi.report.getRange(panel, relative, 'edge', 'Pearson');
    tremppi.cytoscape.mapRange(panel, 'edge[Pearson>=0]', 'Pearson', 'line-color', 0, range.max, 'yellow', 'green');
    tremppi.cytoscape.mapRange(panel, 'edge[Pearson<0]', 'Pearson', 'line-color', range.min, 0, 'red', 'yellow');

    tremppi.cytoscape.mapValue(panel, 'node[Bias>0]', 'border-style', 'solid');
    tremppi.cytoscape.mapValue(panel, 'node[Bias=0]', 'border-style', 'dotted');
    tremppi.cytoscape.mapValue(panel, 'node[Bias<0]', 'border-style', 'dashed');
};

tremppi.correlations.num_of_decimals = 3;

// Adds reactive tip window that appears on mouseover on the edge
tremppi.correlations.addQtip = function (panel) {
    var elements = tremppi.correlations[panel].elements();
    var labeller = function (my_data) {
        if (typeof my_data.name !== 'undefined') {
            return 'name: ' + my_data.name + '<br />'
                    + 'Bias: ' + my_data.Bias.toFixed(tremppi.correlations.num_of_decimals) + '<br />';
        } 
        else {
            return 'source: ' + my_data.source + '<br />'
                    + 'target: ' + my_data.target + '<br />'
                    + 'Correlation: ' + my_data.Pearson.toFixed(tremppi.correlations.num_of_decimals) + '<br />';
        }
    };
    tremppi.qtip.addOnHoverLabeller(panel, elements, labeller);
};

tremppi.correlations.bar_left = 110;
tremppi.correlations.B_height = 40;
tremppi.correlations.C_height = 20;

tremppi.correlations.loadLabels = function (panel) {
    var graph = tremppi.correlations[panel];

    var relative = tremppi.getItem('relative') === 'true';

    var my_paper = new paper.PaperScope();

    var legend_height = Math.min(10 * graph.zoom() + 35, $('#container_' + panel).height() / 3);
    $('#legend_' + panel).height(legend_height);
    my_paper.setup($('#legend_' + panel)[0]);

    my_paper.activate();
    tremppi.correlations.addGradient(relative, panel, my_paper);
    tremppi.correlations.addWidth(relative, panel, my_paper, graph.zoom());
    my_paper.view.draw();
};

tremppi.correlations.addGradient = function (relative, panel, my_paper) {
    var range = tremppi.report.getRange(panel, relative, 'edge', 'Pearson');
    var bar_right = my_paper.view.viewSize.width - 80;

    // Make the bar   
    var bar = new paper.Path.Rectangle(
            new paper.Rectangle(
                    new paper.Point(tremppi.correlations.bar_left, tremppi.correlations.C_height - 10),
                    new paper.Point(bar_right, tremppi.correlations.C_height)));
    bar.fillColor = {gradient: {}};
    bar.fillColor.origin = [tremppi.correlations.bar_left, 0];
    bar.fillColor.destination = [bar_right, 0];
    bar.fillColor.gradient.stops = ['red', 'yellow', 'green'];
    bar.strokeColor = 'black';
    bar.strokeWidth = 1;
    // Make the text
    tremppi.paper.makeText('C: ', new paper.Point(10, tremppi.correlations.C_height));
    tremppi.paper.makeText(
            range.min.toFixed(tremppi.correlations.num_of_decimals),
            new paper.Point(tremppi.correlations.bar_left - 75, tremppi.correlations.C_height)
            );
    tremppi.paper.makeText(
            '0',
            new paper.Point(my_paper.view.viewSize.width/2, tremppi.correlations.C_height)
            );
    tremppi.paper.makeText(
            range.max.toFixed(tremppi.correlations.num_of_decimals),
            new paper.Point(bar_right + 5, tremppi.correlations.C_height)
            );
};

tremppi.correlations.addWidth = function (relative, panel, my_paper, width_ratio) {
    var range = tremppi.report.getRange(panel, relative, 'node', 'Bias');
    var abs_max = Math.max(Math.abs(range.min), Math.abs(range.max));
    var bar_height = tremppi.correlations.B_height - 10;
    var bar_right = my_paper.view.viewSize.width - 80;

    // Create the bar
    var bar = new paper.Path();
    bar.fillColor = 'black';
    bar.strokeWidth = 0;
    var min_width = 1 * width_ratio;
    var max_width = 10 * width_ratio;
    bar.add(new paper.Point(bar_right, bar_height));
    bar.add(new paper.Point(tremppi.correlations.bar_left, bar_height + (max_width - min_width) / 2));
    bar.add(new paper.Point(tremppi.correlations.bar_left, bar_height + (max_width + min_width) / 2));
    bar.add(new paper.Point(bar_right, bar_height + max_width));
    bar.add(new paper.Point(bar_right, bar_height));
    // Add the label
    var F_pad = max_width / 2 - 4;
    tremppi.paper.makeText('B: ', new paper.Point(10, tremppi.correlations.B_height + F_pad));
    tremppi.paper.makeText(
            (0.0).toFixed(tremppi.correlations.num_of_decimals),
            new paper.Point(tremppi.correlations.bar_left - 75, tremppi.correlations.B_height + F_pad)
            );
    tremppi.paper.makeText(
            abs_max.toFixed(tremppi.correlations.num_of_decimals),
            new paper.Point(bar_right + 5, tremppi.correlations.B_height + F_pad)
            );
};
