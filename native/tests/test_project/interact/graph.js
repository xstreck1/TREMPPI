/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.interact.Graph = {
    // Creates a single graph
    makeGraph: function (graph, type) {
        var name = 'graph_' + type;

        for (var edge_no = 0; edge_no < graph.edges.length; edge_no++) {
            var data = graph.edges[edge_no].data;
            data.color_mapper = Math.abs(data.Pearson);
            data.width_mapper = Math.abs(data.Frequency);
            data.weight_mapper = Math.abs(data.Frequency) / data.ExpectedFreq;
            if (type === "differ") {
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

        var selected_col = '#5555BB';
        $('#' + name).cytoscape({
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
                name: tremppi.common.hasAllPositions(graph.nodes) ? 'preset' : 'grid'
            },
            elements: graph
        });
    },
    // Change between relative and absolute values
    applyVisuals: function (config) {
        var rel_string = config.relative ? "relative" : "absolute";
        var width_type = config.weighted ? "weight" : "width";

        // Create the mapper for the graph
        var createMyMapping = function (config, type, rel_string, sign, selection, glyph, mapper) {
            var min = config[type][rel_string][glyph].min;
            var max = config[type][rel_string][glyph].max;
            if (min === max) 
                $('#graph_' + type).cytoscape('get').style().selector(selection).css(mapper, config[type][glyph + sign].max).update();
            else 
                $('#graph_' + type).cytoscape('get').style().selector(selection).css(mapper,
                    'mapData(' + glyph + '_mapper, ' + min + ', ' + max + ', ' +
                    config[type][glyph + sign].min + ', ' + config[type][glyph + sign].max + ')').update();
        };

        var setLabels = function (type) {
            createMyMapping(config, type, rel_string, '', 'edge', width_type, 'width');

            if (type === "differ") {
                createMyMapping(config, type, rel_string, '_pos', 'edge.positive', 'color', 'line-color');
                createMyMapping(config, type, rel_string, '_neg', 'edge.negative', 'color', 'line-color');
            } else {

                createMyMapping(config, type, rel_string, '', 'edge', 'color', 'line-color');
            }
        };

        for (var i = 0; i < config.types.length; i++) {
            setLabels(config.types[i]);
            tremppi.interact.Labels.addQtip($('#graph_' + config.types[i]).cytoscape('get'), config.types[i]);
        }
    }
};
