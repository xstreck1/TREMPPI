/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi, cytoscape, paper */

tremppi.regulations.createPanelContent = function (elements, panel) {
    tremppi.regulations.makeGraph(elements, panel);
    tremppi.regulations[panel].load(elements);
    tremppi.regulations.configure(panel);
    tremppi.regulations.applyVisuals(panel);
    tremppi.regulations[panel].reset();
    tremppi.regulations.addQtip(panel);
    tremppi.regulations.loadLabels(panel);
};

tremppi.regulations.valuesSetter = function (source, panel) {
    return function (data) {
        tremppi.regulations.createPanelContent(data.elements, panel);
        tremppi.log(source + " loaded successfully.");

        if (tremppi.regulations.left.edges().length > 0 && tremppi.regulations.right.edges().length > 0) {
            var mid = {};
            $.extend(true, mid, tremppi.regulations.left.json().elements);
            var right_edges = tremppi.regulations.right.json().elements.edges;
            // Only make diff if both graphs are loaded
            for (var i = 0; i < right_edges.length; i++) {
                var substract = function (value) {
                    mid.edges[i].data[value] -= right_edges[i].data[value]
                };
                substract("Frequency");
                substract("Pearson");
            }
            tremppi.regulations.createPanelContent(mid, 'mid');
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
                    'target-arrow-shape': 'data(target_arrow_shape)',
                    'line-style': 'data(line_style)',
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
        }
    });
};

tremppi.regulations.makeGraph = function (graph, type) {
    for (var edge_no = 0; edge_no < graph.edges.length; edge_no++) {
        var data = graph.edges[edge_no].data;
        data.color_mapper = Math.abs(data.Pearson);
        data.width_mapper = Math.abs(data.Frequency);
        data.weight_mapper = Math.abs(data.Frequency) / data.ExpectedFreq;
        if (type === 'mid') {
            data.target_arrow_shape = 'circle';
            graph.edges[edge_no].classes = data.Pearson >= 0 ? 'positive' : 'negative';
            if (data.Frequency > 0) {
                data.line_style = 'solid';
            } else if (data.Frequency === 0) {
                data.line_style = 'dotted';
            } else {
                data.line_style = 'dashed';
            }
        }
        else {
            data.line_style = 'solid';
            data.target_arrow_shape = data.Pearson >= 0 ? 'triangle' : 'tee';
        }
    }
};

// Create the mapper for the graph
tremppi.regulations.createMyMapping = function (type, rel_string, sign, selection, glyph, mapper) {
    var config = tremppi.regulations.config;
    var min = config[type][rel_string][glyph].min;
    var max = config[type][rel_string][glyph].max;
    if (min === max)
        tremppi.regulations[type].style().selector(selection).css(mapper, config[type][glyph + sign].min).update();
    else {
        var map = 'mapData(' + glyph + '_mapper, ' + min + ', ' + max + ', ' + config[type][glyph + sign].min + ', ' + config[type][glyph + sign].max + ')';
        tremppi.regulations[type].style().selector(selection).css(mapper, map).update();
    }
};

// Change between relative and absolute values
tremppi.regulations.applyVisuals = function (type) {
    var relative = tremppi.getItem("relative") === "true" ? "relative" : "absolute";
    var weighted = tremppi.getItem("weighted") === "true" ? "weight" : "width";

    tremppi.regulations.createMyMapping(type, relative, '', 'edge', weighted, 'width');

    if (type === 'mid') {
        tremppi.regulations.createMyMapping(type, relative, '_pos', 'edge.positive', 'color', 'line-color');
        tremppi.regulations.createMyMapping(type, relative, '_neg', 'edge.negative', 'color', 'line-color');
    } else {
        tremppi.regulations.createMyMapping(type, relative, '', 'edge', 'color', 'line-color');
    }
};

// Adds reactive tip window that appears on mouseover on the edge
tremppi.regulations.addQtip = function (type) {
    var num_of_decimals = 3;

    var edges = tremppi.regulations[type].edges();
    var labeller = function (my_data) {
        return "source: " + my_data.source + "<br />"
                + "target: " + my_data.target + "<br />"
                + "pearson: " + my_data.Pearson.toFixed(num_of_decimals) + "<br />"
                + "frequency: " + my_data.Frequency.toFixed(num_of_decimals) + "<br />"
                + "expected frequency: " + my_data.ExpectedFreq.toFixed(num_of_decimals) + "<br />";
    };

    tremppi.qtip.addOnHoverLabeller(type, edges, labeller);
};

tremppi.regulations.bar_left = 110;
tremppi.regulations.num_of_decimals = 3;
tremppi.regulations.F_height = 40;
tremppi.regulations.P_height = 20;

tremppi.regulations.loadLabels = function (type) {
    var graph = tremppi.regulations[type];

    var relative = tremppi.getItem("relative") === "true" ? "relative" : "absolute";
    var weighted = tremppi.getItem("weighted") === "true" ? "weight" : "width";

    var my_paper = new paper.PaperScope();
    var legend_height = Math.min(tremppi.regulations.config[type][weighted].max * graph.zoom() + 40, $("#container_" + type).height() / 3);
    $('#legend_' + type).height(legend_height);
    my_paper.setup($('#legend_' + type)[0]);

    tremppi.regulations.addEdgeWidth(relative, weighted, type, my_paper, graph.zoom());
    tremppi.regulations.addGradient(relative, weighted, type, my_paper);
};


tremppi.regulations.makeText = function (content, position) {
    var text = new paper.PointText(position);
    text.fillColor = 'black';
    text.fontSize = 20;
    text.fontFamily = 'Courier New';
    text.content = content;
    return text;
};

tremppi.regulations.addEdgeWidth = function (relative, weighted, type, my_paper, width_ratio) {
    my_paper.activate();
    var bar_height = tremppi.regulations.F_height - 10;
    var bar_right = my_paper.view.viewSize.width - 70;

    // Create the bar
    var bar = new paper.Path();
    bar.fillColor = 'black';
    bar.strokeWidth = 0;
    var min_width = tremppi.regulations.config[type][weighted].min * width_ratio;
    var max_width = tremppi.regulations.config[type][weighted].max * width_ratio;
    bar.add(new paper.Point(bar_right, bar_height));
    bar.add(new paper.Point(tremppi.regulations.bar_left, bar_height + (max_width - min_width) / 2));
    bar.add(new paper.Point(tremppi.regulations.bar_left, bar_height + (max_width + min_width) / 2));
    bar.add(new paper.Point(bar_right, bar_height + max_width));
    bar.add(new paper.Point(bar_right, bar_height));
    // Add the label
    var min = tremppi.regulations.config[type][relative][weighted].min.toFixed(tremppi.regulations.num_of_decimals);
    min = min >= 0 ? ' ' + min : min;
    var max = tremppi.regulations.config[type][relative][weighted].max.toFixed(tremppi.regulations.num_of_decimals);
    var F_pad = (tremppi.regulations.config[type][weighted].max * width_ratio) / 2 - 4;
    tremppi.regulations.makeText('F: ', new paper.Point(10, tremppi.regulations.F_height + F_pad));
    tremppi.regulations.makeText(min, new paper.Point(tremppi.regulations.bar_left - 75, tremppi.regulations.F_height + F_pad));
    tremppi.regulations.makeText(max, new paper.Point(bar_right + 5, tremppi.regulations.F_height + F_pad));
    my_paper.view.draw();
};

tremppi.regulations.addGradient = function (relative, weighted, type, my_paper) {
    my_paper.activate();
    var bar_right = my_paper.view.viewSize.width - 70;
    tremppi.regulations.makeText('P: ', new paper.Point(10, tremppi.regulations.P_height));
    // Make the bar   
    var bar = new paper.Path.Rectangle(
            new paper.Rectangle(
                    new paper.Point(tremppi.regulations.bar_left, tremppi.regulations.P_height - 10),
                    new paper.Point(bar_right, tremppi.regulations.P_height)));
    bar.fillColor = {gradient: {}};
    bar.fillColor.origin = [tremppi.regulations.bar_left, 0];
    bar.fillColor.destination = [bar_right, 0];
    if (type === "left")
        bar.fillColor.gradient.stops = ['yellow', 'green'];
    else if (type === 'mid')
        bar.fillColor.gradient.stops = ['red', 'yellow', 'green'];
    else if (type === 'right')
        bar.fillColor.gradient.stops = ['yellow', 'red'];
    bar.strokeColor = 'black';
    bar.strokeWidth = 1;
    // Make the text
    var min = tremppi.regulations.config[type][relative].color.min.toFixed(tremppi.regulations.num_of_decimals);
    min = min >= 0 ? ' ' + min : min;
    var max = tremppi.regulations.config[type][relative].color.max.toFixed(tremppi.regulations.num_of_decimals);
    tremppi.regulations.makeText(min, new paper.Point(tremppi.regulations.bar_left - 75, tremppi.regulations.P_height));
    tremppi.regulations.makeText(max, new paper.Point(bar_right + 5, tremppi.regulations.P_height));

    my_paper.view.draw();
};