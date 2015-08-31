/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi, cytoscape, paper */

tremppi.regulations.createPanelContent = function (elements, panel) {
    tremppi.regulations[panel].load(elements);
    tremppi.regulations.applyVisuals(panel);
    tremppi.regulations.loadLabels(panel);
    tremppi.regulations.addQtip(panel);
};

tremppi.regulations.valuesSetter = function (source, panel) {
    return function (data) {
        $('#header_' + panel).html(source);
        tremppi.regulations.createPanelContent(data.elements, panel);
        tremppi.log(source + ' loaded successfully.');

        if (tremppi.regulations.left.edges().length > 0 && tremppi.regulations.right.edges().length > 0) {
            $('#header_mid').html($('#header_left').html() + ' - ' + $('#header_right').html());
            var to_synchronize = tremppi.regulations.mid.nodes().length === 0;

            var mid = {};
            $.extend(true, mid, tremppi.regulations.left.json().elements);
            var right_edges = tremppi.regulations.right.json().elements.edges;
            // Only make diff if both graphs are loaded
            for (var i = 0; i < right_edges.length; i++) {
                var substract = function (value) {
                    mid.edges[i].data[value] -= right_edges[i].data[value];
                };
                substract('WeightedFrequency');
                substract('Frequency');
                substract('Pearson');
            }
            tremppi.regulations.createPanelContent(mid, 'mid');

            if (to_synchronize) {
                tremppi.cytoscape.synchronize(tremppi.regulations.loadLabels);
            }
        }
    };
};

tremppi.regulations.setPanel = function (panel) {
    var selected_col = '#5555BB';
    tremppi.widget[panel] = cytoscape({
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
};

// Change between relative and absolute values
tremppi.regulations.applyVisuals = function (type) {
    var relative = tremppi.getItem('relative') === 'true';
    var weighted = tremppi.getItem('weighted') === 'true';

    var width_param = weighted ? 'WeightedFrequency' : 'Frequency';
    var range = tremppi.report.getRange(type, relative, 'edge[Frequency]', width_param, true);
    tremppi.cytoscape.mapRange(type, 'edge[Frequency>=0]', width_param, 'width', range.min, range.max, 1, 10);
    tremppi.cytoscape.mapRange(type, 'edge[Frequency<0]', width_param, 'width', range.max * -1, range.min * -1, 10, 1);

    range = tremppi.report.getRange(type, relative, 'edge[Pearson>=0]', 'Pearson', true);
    tremppi.cytoscape.mapRange(type, 'edge[Pearson>=0]', 'Pearson', 'line-color', range.min, range.max, 'yellow', 'green');
    range = tremppi.report.getRange(type, relative, 'edge[Pearson<0]', 'Pearson', false);
    tremppi.cytoscape.mapRange(type, 'edge[Pearson<0]', 'Pearson', 'line-color', range.min, range.max, 'red', 'yellow');

    tremppi.cytoscape.mapValue(type, 'edge[Sign="0"]', 'target-arrow-shape', 'circle');
    tremppi.cytoscape.mapValue(type, 'edge[Sign="+"]', 'target-arrow-shape', 'triangle');
    tremppi.cytoscape.mapValue(type, 'edge[Sign="-"]', 'target-arrow-shape', 'tee');
    tremppi.cytoscape.mapValue(type, 'edge[Sign="1"]', 'target-arrow-shape', 'triangle-tee');

    tremppi.cytoscape.mapValue(type, 'edge[Frequency>0]', 'line-style', 'solid');
    tremppi.cytoscape.mapValue(type, 'edge[Frequency=0]', 'line-style', 'dotted');
    tremppi.cytoscape.mapValue(type, 'edge[Frequency<0]', 'line-style', 'dashed');
};

// Adds reactive tip window that appears on mouseover on the edge
tremppi.regulations.addQtip = function (type) {
    var num_of_decimals = 3;

    var edges = tremppi.regulations[type].edges();
    var labeller = function (my_data) {
        return 'source: ' + my_data.source + '<br />'
                + 'target: ' + my_data.target + '<br />'
                + 'Impact: ' + my_data.Pearson.toFixed(num_of_decimals) + '<br />'
                + 'Frequency: ' + my_data.Frequency.toFixed(num_of_decimals) + '<br />'
                + 'Weighted Frequency: ' + my_data.WeightedFrequency.toFixed(num_of_decimals) + '<br />';
    };

    tremppi.qtip.addOnHoverLabeller(type, edges, labeller);
};

tremppi.regulations.bar_left = 110;
tremppi.regulations.num_of_decimals = 3;
tremppi.regulations.F_height = 60;
tremppi.regulations.N_height = 40;
tremppi.regulations.P_height = 20;

tremppi.regulations.loadLabels = function (type) {
    var graph = tremppi.regulations[type];

    var relative = tremppi.getItem('relative') === 'true';
    var weighted = tremppi.getItem('weighted') === 'true' ? 'WeightedFrequency' : 'Frequency';

    var my_paper = new paper.PaperScope();

    var legend_height = Math.min(10 * graph.zoom() + 55, $('#container_' + type).height() / 3);
    $('#legend_' + type).height(legend_height);
    my_paper.setup($('#legend_' + type)[0]);

    my_paper.activate();
    tremppi.regulations.addGradient(relative, type, my_paper, true);
    tremppi.regulations.addGradient(relative, type, my_paper, false);
    tremppi.regulations.addEdgeWidth(relative, weighted, type, my_paper, graph.zoom());
    my_paper.view.draw();
};

tremppi.regulations.makeText = function (content, position) {
    var text = new paper.PointText(position);
    text.fillColor = 'black';
    text.fontSize = 20;
    text.fontFamily = 'Courier New';
    text.content = content;
    return text;
};

tremppi.regulations.addGradient = function (relative, type, my_paper, positive) {
    var selection = positive ? 'edge[Pearson>=0]' : 'edge[Pearson<0]';
    var range = {
        min: tremppi.report.getRange(type, relative, selection, 'Pearson', positive).min,
        max: tremppi.report.getRange(type, relative, selection, 'Pearson', positive).max
    };
    var bar_right = my_paper.view.viewSize.width - 80;
    var height = positive ? tremppi.regulations.P_height : tremppi.regulations.N_height;

    // Make the bar   
    var bar = new paper.Path.Rectangle(
            new paper.Rectangle(
                    new paper.Point(tremppi.regulations.bar_left, height - 10),
                    new paper.Point(bar_right, height)));
    bar.fillColor = {gradient: {}};
    bar.fillColor.origin = [tremppi.regulations.bar_left, 0];
    bar.fillColor.destination = [bar_right, 0];
    bar.fillColor.gradient.stops = positive ? ['yellow', 'green'] : ['red', 'yellow'];
    bar.strokeColor = 'black';
    bar.strokeWidth = 1;
    // Make the text
    tremppi.regulations.makeText('I: ', new paper.Point(10, height));
    tremppi.regulations.makeText(
            range.min.toFixed(tremppi.regulations.num_of_decimals),
            new paper.Point(tremppi.regulations.bar_left - 75, height)
            );
    tremppi.regulations.makeText(
            range.max.toFixed(tremppi.regulations.num_of_decimals),
            new paper.Point(bar_right + 5, height)
            );
};

tremppi.regulations.addEdgeWidth = function (relative, weighted, type, my_paper, width_ratio) {
    var range = tremppi.report.getRange(type, relative, 'edge', weighted, true);
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
    tremppi.regulations.makeText('F: ', new paper.Point(10, tremppi.regulations.F_height + F_pad));
    tremppi.regulations.makeText(
            range.min.toFixed(tremppi.regulations.num_of_decimals),
            new paper.Point(tremppi.regulations.bar_left - 75, tremppi.regulations.F_height + F_pad)
            );
    tremppi.regulations.makeText(
            range.max.toFixed(tremppi.regulations.num_of_decimals),
            new paper.Point(bar_right + 5, tremppi.regulations.F_height + F_pad)
            );
};
