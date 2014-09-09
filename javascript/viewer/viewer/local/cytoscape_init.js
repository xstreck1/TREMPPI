$(function() { // on dom ready
    var selected_col = '#5555BB';

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
                }).selector(':selected')
                .css({
                    'border-color': selected_col,
                    'line-color': selected_col,
                    'target-arrow-color': selected_col
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
    var edges = graph.elements("edge");

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

    var clickFunction = function(type, id) {
        return function() {
            tremppi_viewer.current_selection = {type: type, name: id};
            tremppi_viewer.setControlScheme("selection");
        };
    };

    // Set node drag reactions to all
    for (var j = 0; j < nodes.length; j++) {
        var selector = '#' + nodes[j].id();
        graph.$(selector).on('tapdrag', moveFunction);
    }

    graph.on('tap', tremppi_viewer.tapFunction);

    tremppi_viewer.activity_type = "selection";
    tremppi_viewer.current_selection = {type: "graph", name: "regulatory"};

    // Set the control scheme
    tremppi_viewer.setButtonControl();

    // Create metadata for selections
    tremppi_viewer.metadata.edge = [];
    tremppi_viewer.metadata.edge.push({name: "id", label: "ID", datatype: "string", editable: true});
    tremppi_viewer.metadata.edge.push({name: "Threshold", label: "THRESHOLD", datatype: "int", editable: true});
    tremppi_viewer.metadata.edge.push({name: "Label", label: "Label", datatype: "string", editable: true});

    tremppi_viewer.metadata.node = [];
    tremppi_viewer.metadata.node.push({name: "id", label: "ID", datatype: "string", editable: true});
    tremppi_viewer.metadata.node.push({name: "MaxActivity", label: "MAX ACTIVITY", datatype: "int", editable: true});
}); // on dom ready