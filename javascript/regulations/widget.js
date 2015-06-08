/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi, cytoscape, paper */

tremppi.regulations.createPanelContent = function (elements, panel) {
    tremppi.regulations.makeGraph(elements, panel);
    tremppi.regulations[panel].load(elements);
    tremppi.regulations.applyVisuals(panel);
    // tremppi.regulations[panel].reset();
    tremppi.regulations.addQtip(panel);
    tremppi.regulations.loadLabels(panel);
};

tremppi.regulations.valuesSetter = function (source, panel) {
    return function (data) {
        tremppi.regulations.createPanelContent(data.elements, panel);
        tremppi.log(source + ' loaded successfully.');

        if (tremppi.regulations.left.edges().length > 0 && tremppi.regulations.right.edges().length > 0) {
            var mid = {};
            $.extend(true, mid, tremppi.regulations.left.json().elements);
            var right_edges = tremppi.regulations.right.json().elements.edges;
            // Only make diff if both graphs are loaded
            for (var i = 0; i < right_edges.length; i++) {
                var substract = function (value) {
                    mid.edges[i].data[value] -= right_edges[i].data[value];
                };
                substract('Frequency');
                substract('Pearson');
            }
            tremppi.regulations.createPanelContent(mid, 'mid');
        }


        tremppi.cytoscape.synchronize(tremppi.regulations.loadLabels);
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

tremppi.regulations.makeGraph = function (graph, panel) {
    for (var edge_no = 0; edge_no < graph.edges.length; edge_no++) {
        var data = graph.edges[edge_no].data;

        data.Width = Math.abs(data.Frequency);
        data.Weight = Math.abs(data.Frequency) / data.ExpectedFreq;

        if (data.Sign === '+') {
            data.target_arrow_shape = 'triangle';
        }
        else if (data.Sign === '-') {
            data.target_arrow_shape = 'tee';
        }
        else if (data.Sign === '1') {
            data.target_arrow_shape = 'triangle-tee';
        }
        else if (data.Sign === '0') {
            data.target_arrow_shape = 'circle';
        }

        if (data.Frequency > 0) {
            data.line_style = 'solid';
        } else if (data.Frequency === 0) {
            data.line_style = 'dotted';
        } else {
            data.line_style = 'dashed';
        }
    }
};

tremppi.regulations.getBound = function (selected, param) {
    var min = 0;
    var max = 0;

    for (var ele_no = 0; ele_no < selected.length; ele_no++) {
        var value = selected[ele_no].data(param);
        min = Math.min(min, value);
        max = Math.max(max, value);
    }

    return {min: min, max: max};
};

tremppi.regulations.getRange = function (type, relative, selection, param) {
    var range;
    if (relative) {
        var selected = tremppi.regulations[type].elements(selection);
        range = tremppi.regulations.getBound(selected, param);
    } else {
        range = {
            min: tremppi.regulations.bounds[param].min, max: tremppi.regulations.bounds[param].max
        };
        if (type === 'mid') {
            range = {min: range.min - range.max, max: range.max - range.min};
        }
    }
    if (range.min < 0) {
        range = {min: 0, max: Math.max(Math.abs(range.min, range.max)) };
    }
    return range;
};

tremppi.regulations.mapValue = function (type, selection, glyph, value) {
    tremppi.regulations[type].style().selector(selection).css(glyph, value).update();
};

// Create the mapper for the graph
tremppi.regulations.mapRange = function (type, selection, param, glyph, min_range, max_range, min_domain, max_domain) {
    if (min_range === max_range) {
        if (min_range < 0) {
            tremppi.regulations.mapValue(type, selection, glyph, max_domain);
        } else {
            tremppi.regulations.mapValue(type, selection, glyph, min_domain);
        }
    }
    else {
        var map = 'mapData(' + param + ', ' + min_range + ', ' + max_range + ', ' + min_domain + ', ' + max_domain + ')';
        tremppi.regulations[type].style().selector(selection).css(glyph, map).update();
    }
};

// Change between relative and absolute values
tremppi.regulations.applyVisuals = function (type) {
    var relative = tremppi.getItem('relative') === 'true';
    var weighted = tremppi.getItem('weighted') === 'true';
    var range = tremppi.regulations.getRange(type, relative, 'edge', weighted ? 'Weight' : 'Width');
    tremppi.regulations.mapRange(type, 'edge', weighted ? 'Weight' : 'Width', 'width', range.min, range.max, 1, 10);
    range = tremppi.regulations.getRange(type, relative, 'edge', 'Pearson');
    tremppi.regulations.mapRange(type, 'edge[Pearson>=0]', 'Pearson', 'line-color', 0, range.max, 'yellow', 'green');
    tremppi.regulations.mapRange(type, 'edge[Pearson<0]', 'Pearson', 'line-color', range.min, 0, 'red', 'yellow');
};

// Adds reactive tip window that appears on mouseover on the edge
tremppi.regulations.addQtip = function (type) {
    var num_of_decimals = 3;

    var edges = tremppi.regulations[type].edges();
    var labeller = function (my_data) {
        return 'source: ' + my_data.source + '<br />'
                + 'target: ' + my_data.target + '<br />'
                + 'pearson: ' + my_data.Pearson.toFixed(num_of_decimals) + '<br />'
                + 'frequency: ' + my_data.Frequency.toFixed(num_of_decimals) + '<br />'
                + 'expected frequency: ' + my_data.ExpectedFreq.toFixed(num_of_decimals) + '<br />';
    };

    tremppi.qtip.addOnHoverLabeller(type, edges, labeller);
};

tremppi.regulations.bar_left = 110;
tremppi.regulations.num_of_decimals = 3;
tremppi.regulations.F_height = 40;
tremppi.regulations.P_height = 20;

tremppi.regulations.loadLabels = function (type) {
    var graph = tremppi.regulations[type];

    var relative = tremppi.getItem('relative') === 'true';
    var weighted = tremppi.getItem('weighted') === 'true' ? 'Weight' : 'Width';

    var my_paper = new paper.PaperScope();

    var legend_height = Math.min(10 * graph.zoom() + 35, $('#container_' + type).height() / 3);
    $('#legend_' + type).height(legend_height);
    my_paper.setup($('#legend_' + type)[0]);

    my_paper.activate();
    tremppi.regulations.addEdgeWidth(relative, weighted, type, my_paper, graph.zoom());
    tremppi.regulations.addGradient(relative, type, my_paper);
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

tremppi.regulations.addGradient = function (relative, type, my_paper) {
    var range = {
        min: tremppi.regulations.getRange(type, relative, 'edge', 'Pearson').max * -1,
        max: tremppi.regulations.getRange(type, relative, 'edge', 'Pearson').max
    };
    var bar_right = my_paper.view.viewSize.width - 70;

    // Make the bar   
    var bar = new paper.Path.Rectangle(
            new paper.Rectangle(
                    new paper.Point(tremppi.regulations.bar_left, tremppi.regulations.P_height - 10),
                    new paper.Point(bar_right, tremppi.regulations.P_height)));
    bar.fillColor = {gradient: {}};
    bar.fillColor.origin = [tremppi.regulations.bar_left, 0];
    bar.fillColor.destination = [bar_right, 0];
    bar.fillColor.gradient.stops = ['red', 'yellow', 'green'];
    bar.strokeColor = 'black';
    bar.strokeWidth = 1;
    // Make the text
    tremppi.regulations.makeText('P: ', new paper.Point(10, tremppi.regulations.P_height));
    tremppi.regulations.makeText(
            range.min.toFixed(tremppi.regulations.num_of_decimals),
            new paper.Point(tremppi.regulations.bar_left - 75, tremppi.regulations.P_height)
            );
    tremppi.regulations.makeText(
            range.max.toFixed(tremppi.regulations.num_of_decimals),
            new paper.Point(bar_right + 5, tremppi.regulations.P_height)
            );
};

tremppi.regulations.addEdgeWidth = function (relative, weighted, type, my_paper, width_ratio) {
    var range = tremppi.regulations.getRange(type, relative, 'edge', weighted);
    var bar_height = tremppi.regulations.F_height - 10;
    var bar_right = my_paper.view.viewSize.width - 70;

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
    tremppi.regulations.makeText('O: ', new paper.Point(10, tremppi.regulations.F_height + F_pad));
    tremppi.regulations.makeText(
            range.min.toFixed(tremppi.regulations.num_of_decimals),
            new paper.Point(tremppi.regulations.bar_left - 75, tremppi.regulations.F_height + F_pad)
            );
    tremppi.regulations.makeText(
            range.max.toFixed(tremppi.regulations.num_of_decimals),
            new paper.Point(bar_right + 5, tremppi.regulations.F_height + F_pad)
            );
};
