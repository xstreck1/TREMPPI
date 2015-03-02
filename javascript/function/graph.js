/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.function.Graph = {
    // Creates a single graph
    makeGraph: function (graph, type) {
        var name = 'graph_' + type;

        for (var edge_no = 0; edge_no < graph.edges.length; edge_no++) {
            var data = graph.edges[edge_no].data;
            data.color_mapper = Math.abs(data.Pearson);
            
            if (type === "differ")
                graph.edges[edge_no].classes = data.Pearson >= 0 ? 'positive' : 'negative';
        }
        
        for (var node_no = 0; node_no < graph.nodes.length; node_no++) {
            var data = graph.nodes[node_no].data;
            data.bias_mapper = Math.abs(data.Bias);
        }

        var selected_col = '#5555BB';
        $('#' + name).cytoscape({
            style: cytoscape.stylesheet()
                    .selector('node')
                    .css({
                        'content': 'data(name)',
                        'text-valign': 'center',
                        'background-color': 'white',
                        'text-outline-color': 'black',
                        'text-outline-width': 1,
                        'color': 'white',
                        'border-color': 'black',
                        'border-width': 1,
                        'shape': 'rectangle',
                        'width': 100,
                        'height': 25,
                        'font-size': 15
                    }).selector('edge').css({
                'width': 5
            })
                    .selector(':selected')
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
            if (type === "differ") {
                createMyMapping(config, type, rel_string, '_pos', 'edge.positive', 'color', 'line-color');
                createMyMapping(config, type, rel_string, '_neg', 'edge.negative', 'color', 'line-color');
            } else {
                createMyMapping(config, type, rel_string, '', 'edge', 'color', 'line-color');
            }
            createMyMapping(config, type, rel_string, '', 'node', 'bias', 'background-color');
        };

        for (var i = 0; i < config.types.length; i++) {
            setLabels(config.types[i]);
            tremppi.function.Labels.addQtip($('#graph_' + config.types[i]).cytoscape('get'), config.types[i]);
        }
    }
};
