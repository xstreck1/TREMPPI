/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi, cytoscape */

tremppi.regulations.valuesSetter = function (source, panel) {
    return function (data) {
        tremppi.regulations.makeGraph(data.elements, panel);
        tremppi.regulations.applyVisuals(panel, "absolute", "weight");
        tremppi.regulations[panel].load(data.elements);
        tremppi.regulations[panel].reset();
        tremppi.regulations.addQtip(panel);
    };
};

tremppi.regulations.setPanel = function (panel) {
    var selected_col = '#5555BB';
    $('#container_' + panel).html("");
    tremppi.widget[panel] = cytoscape({
        container: document.getElementById('container_' + panel),
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
       tremppi.regulations[type].style().selector(selection).css(mapper, config[type][glyph + sign].max).update();
    else
        tremppi.regulations[type].style().selector(selection).css(mapper,
                'mapData(' + glyph + '_mapper, ' + min + ', ' + max + ', ' +
                config[type][glyph + sign].min + ', ' + config[type][glyph + sign].max + ')').update();
};

// Change between relative and absolute values
tremppi.regulations.applyVisuals = function (type, weigth, width) {
    tremppi.regulations.createMyMapping(type, weigth, '', 'edge', width, 'width');

    if (type === 'mid') {
        tremppi.regulations.createMyMapping(type, weigth, '_pos', 'edge.positive', 'color', 'line-color');
        tremppi.regulations.createMyMapping(type, weigth, '_neg', 'edge.negative', 'color', 'line-color');
    } else {
        tremppi.regulations.createMyMapping(type, weigth, '', 'edge', 'color', 'line-color');
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

tremppi.regulations.loadLabels = function () {
    var config = tremppi.data.config;

    var bar_left = 110;
    var num_of_decimals = 3;
    var rel_string = config.relative ? "relative" : "absolute";
    var width_type = config.weighted ? "weight" : "width";

    var cys = [];
    var papers = [];
    for (var id = 0; id < config.types.length; id++) {
        cys[id] = $('#graph_' + config.types[id]).cytoscape('get');
        papers[id] = new paper.PaperScope();
        var legend_height = Math.min(config[config.types[id]][width_type].max * cys[id].zoom() + 40, $("#data").height() / 3);
        $('#legend_' + config.types[id]).height(legend_height);
        papers[id].setup($('#legend_' + config.types[id])[0]);
    }
    var F_height = 40;
    var P_height = 20;

    var makeText = function (content, position) {
        var text = new paper.PointText(position);
        text.fillColor = 'black';
        text.fontSize = 20;
        text.fontFamily = 'Courier New';
        text.content = content;
        return text;
    };

    var addEdgeWidth = function (type, paper, width_ratio) {
        paper.activate();
        var bar_height = F_height - 10;
        var bar_right = paper.view.viewSize.width - 70;

        // Create the bar
        var bar = new paper.Path();
        bar.fillColor = 'black';
        bar.strokeWidth = 0;
        var min_width = config[type][width_type].min * width_ratio;
        var max_width = config[type][width_type].max * width_ratio;
        bar.add(new paper.Point(bar_right, bar_height));
        bar.add(new paper.Point(bar_left, bar_height + (max_width - min_width) / 2));
        bar.add(new paper.Point(bar_left, bar_height + (max_width + min_width) / 2));
        bar.add(new paper.Point(bar_right, bar_height + max_width));
        bar.add(new paper.Point(bar_right, bar_height));
        // Add the label
        var min = config[type][rel_string][width_type].min.toFixed(num_of_decimals);
        min = min >= 0 ? ' ' + min : min;
        var max = config[type][rel_string][width_type].max.toFixed(num_of_decimals);
        var F_pad = (config[type][width_type].max * width_ratio) / 2 - 4;
        makeText('F: ', new paper.Point(10, F_height + F_pad));
        makeText(min, new paper.Point(bar_left - 75, F_height + F_pad));
        makeText(max, new paper.Point(bar_right + 5, F_height + F_pad));
        paper.view.draw();
    };

    var addGradient = function (type, paper, id) {
        paper.activate();
        var bar_right = paper.view.viewSize.width - 70;
        makeText('P: ', new paper.Point(10, P_height));
        // Make the bar   
        var bar = new paper.Path.Rectangle(
                new paper.Rectangle(
                        new paper.Point(bar_left, P_height - 10),
                        new paper.Point(bar_right, P_height)));
        bar.fillColor = {gradient: {}};
        bar.fillColor.origin = [bar_left, 0];
        bar.fillColor.destination = [bar_right, 0];
        if (id === 0)
            bar.fillColor.gradient.stops = ['yellow', 'green'];
        else if (id === 1)
            bar.fillColor.gradient.stops = ['red', 'yellow', 'green'];
        else if (id === 2)
            bar.fillColor.gradient.stops = ['yellow', 'red'];
        bar.strokeColor = 'black';
        bar.strokeWidth = 1;
        // Make the text
        var min = config[type][rel_string].color.min.toFixed(num_of_decimals);
        min = min >= 0 ? ' ' + min : min;
        var max = config[type][rel_string].color.max.toFixed(num_of_decimals);
        makeText(min, new paper.Point(bar_left - 75, P_height));
        makeText(max, new paper.Point(bar_right + 5, P_height));

        paper.view.draw();
    };

    // Assign the papers
    for (var id = 0; id < config.types.length; id++) {
        addEdgeWidth(config.types[id], papers[id], cys[id].zoom());
        addGradient(config.types[id], papers[id], id);
    }
};
