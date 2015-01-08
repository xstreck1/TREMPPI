tremppi_witness.Witness = {// on dom ready
    load: function () {
        $("#graph_holder").html("");
        for (var prop_name in witness) {
            if (prop_name !== "setup") {
                $("#graph_holder").append('<div class="prop_name">' + prop_name + '</div></br>');
                $("#graph_holder").append('<div class="cy" id = "' + prop_name + '"></div></br>');
                $('#' + prop_name).cytoscape({
                    layout: {
                        name: tremppi.common.hasAllPositions(witness[prop_name].elements.nodes) ? 'preset' : 'breadthfirst',
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
                                'color': 'white',
                                'target-arrow-color': 'black',
                                'width': 2
                            }),
                    elements: witness[prop_name].elements,
                    ready: (function (prop_name) {
                        return function () {
                            var graph = this;
                            for (var j = 0; j < graph.nodes().length; j++) {
                                var id = '#' + graph.nodes()[j].id();
                                this.$(id).on('free', function () {
                                    var elems = graph.json().elements;
                                    witness[prop_name]["elements"] = elems;
                                    tremppi.common.save("witness");
                                });
                            }
                        };
                    })(prop_name)
                });
            }
        }
    }
};