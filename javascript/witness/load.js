tremppi.witness.load = function () {


    $("#graph_holder").html("");
    for (var prop_name in tremppi.data) {
        if (prop_name !== "setup") {
            $("#graph_holder").append('<div class="prop_name">' + prop_name + '</div></br>');
            $("#graph_holder").append('<div class="cy" id = "' + prop_name + '"></div></br>');
            $('#' + prop_name).cytoscape({
                layout: {
                    name: tremppi.common.hasAllPositions(tremppi.data[prop_name].elements.nodes) ? 'preset' : 'breadthfirst',
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
                elements: tremppi.data[prop_name].elements,
                ready: (function (prop_name) {
                    return function () {
                        var graph = this;
                        for (var j = 0; j < graph.nodes().length; j++) {
                            var id = '#' + graph.nodes()[j].id();
                            this.$(id).on('free', function () {
                                var elems = graph.json().elements;
                                tremppi.data[prop_name]["elements"] = elems;
                                tremppi.common.save();
                            });
                        }
                    };
                })(prop_name)
            });

            tremppi.witness.addQtip(prop_name);
        }
    }
};

tremppi.witness.addQtip = function (div_id) {
    var graph = $("#" + div_id).cytoscape('get');

    var elements = graph.filter("");
    var labeller = function (my_data) {
        var text = "";
        // Is edge
        if (my_data.source) {
            for (var i = 0; i < my_data.source.length - 1; i++) {
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
            for (var i = 0; i < my_data.id.length; i++) {
                if (i === my_data.id.length - 1)
                    text += "auto";
                else
                    text += tremppi.data.setup.components[i];
                text += ": " + my_data.id[i] + "<br />";
            }
        }
        return text;
    };
    tremppi.qtip.addOnHoverLabeller(div_id, elements, labeller);
};