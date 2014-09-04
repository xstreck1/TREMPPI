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
        elements: addIDs(elements),
        ready: function() {
            for (var i = 0; i < elements.nodes.length; i++) {
                var pos = {};
                if (elements.nodes[i].x)
                    pos.x = elements.nodes[i].x;
                if (elements.nodes[i].y)
                    pos.y = elements.nodes[i].y;
                var filter = 'node[id = "' + elements.nodes[i].data.id + '"]'
                var selected = this.filter(filter);
                selected.position(pos);
            }
            var PADDING = 15;
            this.fit(this.filter('node'), PADDING);
        }
    });
}); // on dom ready