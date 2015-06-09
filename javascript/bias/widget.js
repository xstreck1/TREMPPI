/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi, cytoscape, paper */

tremppi.bias.createPanelContent = function (elements, panel) {
    tremppi.bias[panel].load(elements);
    tremppi.bias.applyVisuals(panel);
    tremppi.bias.loadLabels(panel);
    tremppi.bias.addQtip(panel);
};

tremppi.bias.valuesSetter = function (source, panel) {
    return function (data) {
        tremppi.bias.createPanelContent(data.elements, panel);
        tremppi.log(source + ' loaded successfully.');

        if (tremppi.bias.left.nodes().length > 0 && tremppi.bias.right.nodes().length > 0) {
            var to_synchronize = tremppi.bias.mid.nodes().length === 0;

            var mid = {};
            $.extend(true, mid, tremppi.bias.left.json().elements);
            var right_edges = tremppi.bias.right.json().elements.edges;
            for (var i = 0; i < right_edges.length; i++) {
                mid.edges[i].data['Pearson'] -= right_edges[i].data['Pearson'];
            }
            var right_nodes = tremppi.bias.right.json().elements.nodes;
            for (var i = 0; i < right_nodes.length; i++) {
                mid.nodes[i].data['Bias'] -= right_nodes[i].data['Bias'];
            }
            
            tremppi.bias.createPanelContent(mid, 'mid');

            if (to_synchronize) {
                tremppi.cytoscape.synchronize(tremppi.bias.loadLabels);
            }
        }
    };
};

tremppi.bias.setPanel = function (panel) {
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
                    'width': 5
                }).selector(':selected')
                .css({
                    'border-color': selected_col,
                    'line-color': selected_col,
                    'target-arrow-color': selected_col
                }),
        layout: {
            name: 'grid'
        }
    });
};

// Change between relative and absolute values
tremppi.bias.applyVisuals = function (type) {
    var relative = tremppi.getItem('relative') === 'true';

    var range = tremppi.report.getRange(type, relative, 'edge[Pearson>=0]', 'Pearson', true);
    tremppi.cytoscape.mapRange(type, 'edge[Pearson>=0]', 'Pearson', 'line-color', range.min, range.max, 'yellow', 'green');
    range = tremppi.report.getRange(type, relative, 'edge[Pearson<0]', 'Pearson', false);
    tremppi.cytoscape.mapRange(type, 'edge[Pearson<0]', 'Pearson', 'line-color', range.min, range.max, 'red', 'yellow');

    var range = tremppi.report.getRange(type, relative, 'node[Bias>=0]', 'Bias', true);
    tremppi.cytoscape.mapRange(type, 'node[Bias>=0]', 'Bias', 'border-width', range.min, range.max, 1, 10);
    range = tremppi.report.getRange(type, relative, 'node[Bias<0]', 'Bias', false);
    tremppi.cytoscape.mapRange(type, 'node[Bias<0]', 'Bias', 'border-width', range.min, range.max, 10, 1);

    tremppi.cytoscape.mapValue(type, 'node[Bias>0]', 'border-style', 'solid');
    tremppi.cytoscape.mapValue(type, 'node[Bias=0]', 'border-style', 'dotted');
    tremppi.cytoscape.mapValue(type, 'node[Bias<0]', 'border-style', 'dashed');
};

tremppi.bias.num_of_decimals = 3;

// Adds reactive tip window that appears on mouseover on the edge
tremppi.bias.addQtip = function (type) {
    var edges = tremppi.bias[type].edges();
    var edge_labeller = function (my_data) {
        return 'source: ' + my_data.source + '<br />'
                + 'target: ' + my_data.target + '<br />'
                + 'Pearson: ' + my_data.Pearson.toFixed(tremppi.bias.num_of_decimals) + '<br />';
    };
    tremppi.qtip.addOnHoverLabeller(type, edges, edge_labeller);

    var nodes = tremppi.bias[type].nodes();
    var node_labeller = function (my_data) {
        return 'name: ' + my_data.name + '<br />'
                + 'Bias: ' + my_data.Bias.toFixed(tremppi.bias.num_of_decimals) + '<br />';
    };
    tremppi.qtip.addOnHoverLabeller(type, nodes, node_labeller);
};

tremppi.bias.bar_left = 110;
tremppi.bias.F_height = 60;
tremppi.bias.N_height = 40;
tremppi.bias.P_height = 20;

tremppi.bias.loadLabels = function (type) {
    var graph = tremppi.bias[type];

    var relative = tremppi.getItem('relative') === 'true';

    var my_paper = new paper.PaperScope();

    var legend_height = Math.min(10 * graph.zoom() + 55, $('#container_' + type).height() / 3);
    $('#legend_' + type).height(legend_height);
    my_paper.setup($('#legend_' + type)[0]);

    my_paper.activate();
    tremppi.bias.addGradient(relative, type, my_paper, true);
    tremppi.bias.addGradient(relative, type, my_paper, false);
    tremppi.bias.addWidth(relative, type, my_paper, graph.zoom());
    my_paper.view.draw();
};

tremppi.bias.makeText = function (content, position) {
    var text = new paper.PointText(position);
    text.fillColor = 'black';
    text.fontSize = 20;
    text.fontFamily = 'Courier New';
    text.content = content;
    return text;
};

tremppi.bias.addGradient = function (relative, type, my_paper, positive) {
    var selection = positive ? 'edge[Pearson>=0]' : 'edge[Pearson<0]';
    var range = {
        min: tremppi.report.getRange(type, relative, selection, 'Pearson', positive).min,
        max: tremppi.report.getRange(type, relative, selection, 'Pearson', positive).max
    };
    var bar_right = my_paper.view.viewSize.width - 80;
    var height = positive ? tremppi.bias.P_height : tremppi.bias.N_height;

    // Make the bar   
    var bar = new paper.Path.Rectangle(
            new paper.Rectangle(
                    new paper.Point(tremppi.bias.bar_left, height - 10),
                    new paper.Point(bar_right, height)));
    bar.fillColor = {gradient: {}};
    bar.fillColor.origin = [tremppi.bias.bar_left, 0];
    bar.fillColor.destination = [bar_right, 0];
    bar.fillColor.gradient.stops = positive ? ['yellow', 'green'] : ['red', 'yellow'];
    bar.strokeColor = 'black';
    bar.strokeWidth = 1;
    // Make the text
    tremppi.bias.makeText('C: ', new paper.Point(10, height));
    tremppi.bias.makeText(
            range.min.toFixed(tremppi.bias.num_of_decimals),
            new paper.Point(tremppi.bias.bar_left - 75, height)
            );
    tremppi.bias.makeText(
            range.max.toFixed(tremppi.bias.num_of_decimals),
            new paper.Point(bar_right + 5, height)
            );
};

tremppi.bias.addWidth = function (relative, type, my_paper, width_ratio) {
    var range = tremppi.report.getRange(type, relative, 'node', 'Bias', true);
    var bar_height = tremppi.bias.F_height - 10;
    var bar_right = my_paper.view.viewSize.width - 80;

    // Create the bar
    var bar = new paper.Path();
    bar.fillColor = 'black';
    bar.strokeWidth = 0;
    var min_width = 1 * width_ratio;
    var max_width = 10 * width_ratio;
    bar.add(new paper.Point(bar_right, bar_height));
    bar.add(new paper.Point(tremppi.bias.bar_left, bar_height + (max_width - min_width) / 2));
    bar.add(new paper.Point(tremppi.bias.bar_left, bar_height + (max_width + min_width) / 2));
    bar.add(new paper.Point(bar_right, bar_height + max_width));
    bar.add(new paper.Point(bar_right, bar_height));
    // Add the label
    var F_pad = max_width / 2 - 4;
    tremppi.bias.makeText('B: ', new paper.Point(10, tremppi.bias.F_height + F_pad));
    tremppi.bias.makeText(
            range.min.toFixed(tremppi.bias.num_of_decimals),
            new paper.Point(tremppi.bias.bar_left - 75, tremppi.bias.F_height + F_pad)
            );
    tremppi.bias.makeText(
            range.max.toFixed(tremppi.bias.num_of_decimals),
            new paper.Point(bar_right + 5, tremppi.bias.F_height + F_pad)
            );
};