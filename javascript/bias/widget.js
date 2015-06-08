/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi, cytoscape, paper */

tremppi.bias.createPanelContent = function (elements, panel) {
    tremppi.bias.makeGraph(elements, panel);
    tremppi.bias[panel].load(elements);
    tremppi.bias.configure(panel, elements);
    tremppi.bias.applyVisuals(panel);
    tremppi.bias[panel].reset();
    tremppi.bias.addQtip(panel);
    tremppi.bias.loadLabels(panel);
};

tremppi.bias.valuesSetter = function (source, panel) {
    return function (data) {
        tremppi.bias.createPanelContent(data.elements, panel);
        tremppi.log(source + " loaded successfully.");

        if (tremppi.bias.left.edges().length > 0 && tremppi.bias.right.edges().length > 0) {
            var mid = {};
            $.extend(true, mid, tremppi.bias.left.json().elements);
            var right_edges = tremppi.bias.right.json().elements.edges;
            var right_nodes = tremppi.bias.right.json().elements.nodes;
            // Only make diff if both graphs are loaded
            for (var i = 0; i < right_edges.length; i++) {
                mid.edges[i].data.Pearson -= right_edges[i].data.Pearson;
            }
            for (var i = 0; i < right_nodes.length; i++) {
                mid.nodes[i].data.Pearson -= right_nodes[i].data.Pearson;
            }
            tremppi.bias.createPanelContent(mid, 'mid');
        }

        tremppi.cytoscape.synchronize(tremppi.bias.loadLabels);
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
                    'border-color': 'black',
                    'border-width': 1,
                    'background-color': 'white',
                    'shape': 'rectangle',
                    'width': 100,
                    'height': 25,
                    'font-size': 15,
                    'text-outline-color': 'black',
                    'text-outline-width': 1,
                    'color': 'white'
                }).selector('edge')
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

tremppi.bias.makeGraph = function (graph) {
    for (var edge_no = 0; edge_no < graph.edges.length; edge_no++) {
        var data = graph.edges[edge_no].data;
        data.edge_pos_mapper = data.Pearson;
        data.edge_neg_mapper = data.Pearson;
        graph.edges[edge_no].classes = data.Pearson >= 0 ? 'positive' : 'negative';
    }
    for (var node_no = 0; node_no < graph.nodes.length; node_no++) {
        var data = graph.nodes[node_no].data;
        data.node_pos_mapper = data.Bias;
        data.node_neg_mapper = data.Bias;
        graph.nodes[node_no].classes = data.Bias >= 0 ? 'positive' : 'negative';
    }
};

// Create the mapper for the graph
tremppi.bias.createMyMapping = function (type, rel_string, selection, glyph, mapper) {
    var config = tremppi.bias.config;
    var min = config[type][rel_string][glyph].min;
    var max = config[type][rel_string][glyph].max;
    if (min === max) {
        if (min >= 0) {
            tremppi.bias[type].style().selector(selection).css(mapper, config[type][glyph].min).update();
        }
        else {
            tremppi.bias[type].style().selector(selection).css(mapper, config[type][glyph].max).update();
        }
    }
    else {
        var map = 'mapData(' + glyph + '_mapper, ' + min + ', ' + max + ', ' + config[type][glyph].min + ', ' + config[type][glyph].max + ')';
        tremppi.bias[type].style().selector(selection).css(mapper, map).update();
    }
};

// Change between relative and absolute values
tremppi.bias.applyVisuals = function (type) {
    var relative = tremppi.getItem("relative") === "true" ? "relative" : "absolute";

    tremppi.bias.createMyMapping(type, relative, 'node.positive', 'node_pos', 'background-color');
    tremppi.bias.createMyMapping(type, relative, 'node.negative', 'node_neg', 'background-color');
    tremppi.bias.createMyMapping(type, relative, 'edge.positive', 'edge_pos', 'line-color');
    tremppi.bias.createMyMapping(type, relative, 'edge.negative', 'edge_neg', 'line-color');
};

// Adds reactive tip window that appears on mouseover on the edge
tremppi.bias.addQtip = function (type) {
    var num_of_decimals = 3;

    var edges = tremppi.bias[type].edges();
    var edge_labeller = function (my_data) {
        return "source: " + my_data.source + "<br />"
                + "target: " + my_data.target + "<br />"
                + "pearson: " + my_data.Pearson.toFixed(tremppi.bias.num_of_decimals ) + "<br />";
    };
    tremppi.qtip.addOnHoverLabeller(type, edges, edge_labeller);
    
    var nodes = tremppi.bias[type].nodes();
    var node_labeller = function(my_data) {
        return "name: " + my_data.name + "<br />"
                + "bias: " + my_data.Bias.toFixed(tremppi.bias.num_of_decimals ) + "<br />";
    };
    tremppi.qtip.addOnHoverLabeller(type, nodes, node_labeller);
};

tremppi.bias.bar_left = 110;
tremppi.bias.num_of_decimals = 3;

tremppi.bias.loadLabels = function (type) {
    var relative = tremppi.getItem("relative") === "true" ? "relative" : "absolute";

    var my_paper = new paper.PaperScope();

    my_paper.setup($('#legend_' + type)[0]);
    my_paper.activate();
    tremppi.bias.addGradient(relative, type, my_paper, "edge", 40);
    tremppi.bias.addGradient(relative, type, my_paper, "node", 20);
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

tremppi.bias.addGradient = function (relative, type, my_paper, element_type, height) {
    var bar_right = my_paper.view.viewSize.width - 70;
    tremppi.bias.makeText(element_type === 'node' ? 'B: ' : 'C: ' , new paper.Point(10, height));
    // Make the bar   
    var bar = new paper.Path.Rectangle(
            new paper.Rectangle(
                    new paper.Point(tremppi.bias.bar_left, height - 10),
                    new paper.Point(bar_right, height)
                    )
            );
    bar.fillColor = {gradient: {}};
    bar.fillColor.origin = [tremppi.bias.bar_left, 0];
    bar.fillColor.destination = [bar_right, 0];
    bar.fillColor.gradient.stops = ['red', tremppi.bias[element_type + '_null_color'], 'green'];
    bar.strokeColor = 'black';
    bar.strokeWidth = 1;
    // Make the text
    var min = tremppi.bias.config[type][relative][element_type + '_neg'].min.toFixed(tremppi.bias.num_of_decimals);
    min = min >= 0 ? ' ' + min : min;
    var max = tremppi.bias.config[type][relative][element_type + '_pos'].max.toFixed(tremppi.bias.num_of_decimals);
    tremppi.bias.makeText(min, new paper.Point(tremppi.bias.bar_left - 75, height));
    tremppi.bias.makeText(max, new paper.Point(bar_right + 5, height));
};