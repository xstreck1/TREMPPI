$(function() { // on dom ready
    var addIDs = function(network) {

        return network;
    };

    $('#graph_object').cytoscape({
        style: cytoscape.stylesheet()
                .selector('node')
                .css({
                    'content': 'data(id)',
                    'text-valign': 'center',
                    'color': 'white',
                    'text-outline-width': 2,
                    'text-outline-color': '#888'
                })
                .selector('edge')
                .css({
                    'target-arrow-shape': 'triangle',
                    'content': 'data(score)'
                })
                .selector(':selected')
                .css({
                    'background-color': 'black',
                    'line-color': 'black',
                    'target-arrow-color': 'black',
                    'source-arrow-color': 'black'
                })
                .selector('.faded')
                .css({
                    'opacity': 0.25,
                    'text-opacity': 0
                }),
        elements: addIDs(network),
        layout: {
            name: 'breadthfirst',
            fit: false, // whether to fit the viewport to the graph
            ready: undefined, // callback on layoutready
            stop: undefined, // callback on layoutstop
            directed: true, // whether the tree is directed downwards (or edges can point in any direction if false)
            padding: 0, // padding on fit
            circle: false, // put depths in concentric circles if true, put depths top down if false
            roots: undefined, // the roots of the trees
            maximalAdjustments: 0 // how many times to try to position the nodes in a maximal way (i.e. no backtracking)
        },
        ready: function() {
            window.cy = this;
            // giddy up...

            cy.elements().unselectify();

            cy.on('tap', 'node', function(e) {
                var node = e.cyTarget;
                var neighborhood = node.neighborhood().add(node);

                cy.elements().addClass('faded');
                neighborhood.removeClass('faded');
            });

            cy.on('tap', function(e) {
                if (e.cyTarget === cy) {
                    cy.elements().removeClass('faded');
                }
            });
        }
    });
}); // on dom ready