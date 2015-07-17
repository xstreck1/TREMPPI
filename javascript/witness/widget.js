/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi, cytoscape */

tremppi.witness.valuesSetter = function (source, panel) {
    return function (data) {
        tremppi.witness[panel].load(data.elements);
        tremppi.log(source + ' loaded successfully.');

        if (tremppi.witness.left.edges().length > 0 && tremppi.witness.right.edges().length > 0) {
            var to_synchronize = tremppi.witness.mid.nodes().length === 0;

            var mid = {};
            $.extend(true, mid, tremppi.witness.left.json().elements);
            var right_edges = tremppi.witness.right.json().elements.edges;
            
            tremppi.witness['mid'].load(mid);
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