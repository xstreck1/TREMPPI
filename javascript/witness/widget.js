/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi, cytoscape */

tremppi.witness.valuesSetter = function (source, panel) {
    return function (data) {
        if (typeof data.elements === 'undefined' || typeof data.elements.nodes === 'undefined' || typeof data.elements.edges === 'undefined') {
            tremppi.log('The witness file does not contain a graph.', 'warning');
            return;
        } 
        
        $('#header_' + panel).html(source);
        tremppi.witness[panel].load(data.elements);
        tremppi.log(source + ' loaded successfully.');

        if (tremppi.witness.left.edges().length > 0 && tremppi.witness.right.edges().length > 0)
        {
            $('#header_mid').html($('#header_left').html() + ' - ' + $('#header_right').html());
            var to_synchronize = tremppi.witness.mid.nodes().length === 0;

            var mid = {nodes: [], edges: []};
            var left_edges = tremppi.witness.left.json().elements.edges;
            var right_edges = tremppi.witness.right.json().elements.edges;

            // Add the edges from left not found in the right
            // Is quadratic, could be improved
            for (var i = 0; i < left_edges.length; i++) {
                var found = false;
                for (var j = 0; j < right_edges.length; j++) {
                    if (left_edges[i].data.Step === right_edges[j].data.Step &&
                            left_edges[i].data.source === right_edges[j].data.source &&
                            left_edges[i].data.target === right_edges[j].data.target)
                    {
                        found = true;
                        break;
                    }
                }
                // Insert the newly found edge
                if (!found) {
                    mid.edges.push({
                        data: {
                            id: left_edges[i].data.id,
                            Step: left_edges[i].data.Step,
                            source: left_edges[i].data.source,
                            target: left_edges[i].data.target
                        }
                    });
                }
            }
            
            var left_nodes = tremppi.witness.left.json().elements.nodes;
            var mid_edges = tremppi.witness.left.json().elements.edges;
            
            // Add nodes that are found in any of the edges:
            // Is |V|*|E|, could be improved
            for (var i = 0; i < left_nodes.length; i++) {
                var found = false;
                for (var j = 0; j < mid_edges.length; j++) {
                    if (mid_edges[j].data.source === left_nodes[i].data.id && mid_edges[j].data.target === left_nodes[i].data.id) {
                        found = true;
                        break;
                    }
                }
                // Insert the newly found edge
                if (found) {
                    mid.nodes.push({
                        data: {
                            id: left_nodes[i].data.id
                        }
                    });
                }
            }

            tremppi.witness['mid'].load(mid);

            if (to_synchronize) {
                tremppi.cytoscape.synchronize(function () {});
            }
        }
    };
};

tremppi.witness.setPanel = function (panel) {
    var selected_col = '#5555BB';
    tremppi.widget[panel] = cytoscape({
        container: document.getElementById('graph_' + panel),
        style: cytoscape.stylesheet()
                .selector('node')
                .css({
                    'content': 'data(id)',
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
                    'content': 'data(Step)',
                    'text-outline-color': 'black',
                    'text-outline-width': 2,
                    'color': 'white',
                    'font-size': 20,
                    'target-arrow-shape': 'triangle'
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