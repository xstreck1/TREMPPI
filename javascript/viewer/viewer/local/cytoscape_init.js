$(function() { // on dom ready
    $('#graph_object').cytoscape({
        style: cytoscape.stylesheet()
                .selector('node')
                .css({
                    'content': 'data(id)',
                    'text-valign': 'center',
                    'background-color': 'white',
                    'border-color': 'black',
                    'border-width': 2,
                    'width': 50,
                    'height': 50
                })
                .selector('edge')
                .css({
                    'target-arrow-shape': 'triangle',
                    'content': 'data(score)',
                    'target-arrow-color': 'black',
                    'line-color': 'black',
                    'width': 2.5
                }),
        elements: elements,
        ready: function() {
            tremppi_viewer.positionNodes(this);
            var PADDING = 15;
            this.fit(this.filter('node'), PADDING);
        }
    });

    var graph = $("#graph_object").cytoscape('get');
    var nodes = graph.elements("node");

    // Sets all nodes with the id to the position given by graph
    var moveFunction = function(event) {
        var target = event.cyTarget;
        var id = target.id();
        if (target.isNode()) {
            var elem = tremppi_viewer.getByID("nodes", id);
            elem.x = target.position('x');
            elem.y = target.position('y');
        }
    };

    // Set node drag reactions to all
    for (var j = 0; j < nodes.length; j++) {
        var id = '#' + nodes[j].id();
        graph.$(id).on('drag', moveFunction);
    }
}); // on dom ready