/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi_report.Graph = {
    // Synchronization in between the graphs
    synchronize: function (config, labelRefresh) {
        var cys = [];
        for (var id = 0; id < config.types.length; id++) {
            cys[id] = $('#reg_graph_' + config.types[id]).cytoscape('get');
        }
        var nodes = cys[0].elements("node");

        // Sets all nodes with the id to the position given by graph
        var moveFunction = function (graph, id) {
            return function (evt) {
                for (var i = 0; i < config.types.length; i++) {
                    cys[i].$(id).renderedPosition(graph.$(id).renderedPosition());
                    report["Regulatory"][config.types[i]]["elements"] = cys[i].json().elements;
                }
                tremppi_common.save("report");
            };
        };

        // Set node drag reactions to all
        for (var j = 0; j < nodes.length; j++) {
            var id = '#' + nodes[j].id();
            for (var i = 0; i < config.types.length; i++) {
                cys[i].$(id).on('drag', moveFunction(cys[i], id));
            }
        }

        // Create zooming function
        var zoomFunction = function (graph, id) {
            return function (evt) {
                for (i = 0; i < config.types.length; i++) {
                    if ((id === i)
                            || (cys[i].zoom() === graph.zoom())
                            || (cys[i].pan() === graph.pan()))
                        continue;
                    cys[i].pan(graph.pan());
                    cys[i].zoom(graph.zoom());
                }
                labelRefresh();
            };
        };

        var panFunction = function (graph, id) {
            return function (evt) {
                for (i = 0; i < config.types.length; i++) {
                    if ((id === i) || (cys[i].pan() === graph.pan()))
                        continue;
                    cys[i].pan(graph.pan());
                }
                ;
            };
        };
        for (var i = 0; i < config.types.length; i++) {
            cys[i].on('zoom', zoomFunction(cys[i], i));
        }

        for (var i = 0; i < config.types.length; i++) {
            cys[i].on('mouseup', panFunction(cys[i], i));
        }
    },
    // Creates a single graph
    makeGraph: function (graph, type) {
        var name = 'reg_graph_' + type;

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
            else
                data.target_arrow_shape = data.Pearson >= 0 ? 'triangle' : 'tee';
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
                name: tremppi_common.hasAllPositions(graph.nodes) ? 'preset' : 'grid'
            },
            elements: graph
        });
    },
    // Change between relative and absolute values
    labelSwitch: function (config, relative, weighted) {
        var rel_string = relative ? "relative" : "absolute";
        var width_type = weighted ? "weight" : "width";

        // Create the mapper for the graph
        var createMyMapping = function (config, type, rel_string, sign, selection, glyph, mapper) {
            var min = config[type][rel_string][glyph].min;
            var max = config[type][rel_string][glyph].max;
            $('#reg_graph_' + type).cytoscape('get').style().selector(selection).css(mapper,
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
            tremppi_report.Labels.addQtip($('#reg_graph_' + config.types[i]).cytoscape('get'), config.types[i]);
        }
    }
};
