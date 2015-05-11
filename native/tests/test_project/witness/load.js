tremppi.witness.load = function () {
    var graph = $("#graph_object").cytoscape({
        layout: {
            name: tremppi.common.hasAllPositions(tremppi.data.elements.nodes) ? 'preset' : 'breadthfirst',
            circle: true
        },
        style: cytoscape.stylesheet().selector('node')
                .css({
                    'text-valign': 'center',
                    'background-color': 'white',
                    'border-color': 'black',
                    'border-width': 1,
                    'shape': 'rectangle',
                    'content': 'data(id)',
                    'width': 100,
                    'height': 25,
                    'font-size': 15
                })
                .selector('edge')
                .css({
                    'target-arrow-shape': 'triangle',
                    'text-outline-color': 'black',
                    'text-outline-width': 1,
					'content': 'data(Step)',
                    'color': 'white',
                    'target-arrow-color': 'black',
                    'width': 2
                }),
        elements: tremppi.data.elements,
        ready: function () {
            var graph = this;
            for (var j = 0; j < graph.nodes().length; j++) {
                var id = '#' + graph.nodes()[j].id();
                this.$(id).on('free', function () {
                    var elems = graph.json().elements;
                    tremppi.data.elements = elems;
                    tremppi.common.save();
                });
            }
        }
    });

    // Add setup data
    tremppi.report.addSetup(tremppi.data.setup);

    // Add labels
    tremppi.witness.addQtip(graph);
};

tremppi.witness.addQtip = function (graph) {
    var getValues = function (data) {
        return data.split("-")[0];
    };

    var elements = graph.filter("");
    var labeller = function (my_data) {
        var text = "";
        // Is edge
        if (my_data.source) {
            for (var i = 0; i < getValues(my_data.source).length; i++) {
                if (my_data.source[i] !== my_data.target[i]) {
                    text += tremppi.data.setup.components[i];
                    text += ": " + my_data.source[i] + " => " + my_data.target[i] + "<br />";
                    break;
                }
            }
            if (text === "")
                text = "steady";
        }
        // Is node
        else {
            for (var i = 0; i < getValues(my_data.id).length; i++) {
                text += tremppi.data.setup.components[i];
                text += ": " + my_data.id[i] + "<br />";
            }
        }
        return text;
    };
    tremppi.qtip.addOnHoverLabeller("graph_object", elements, labeller);
};