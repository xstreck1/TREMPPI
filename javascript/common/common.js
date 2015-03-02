tremppi.common = {
};
tremppi.qtip = {
    getConfig: function () {
        this.position = {
            target: 'mouse', // Position it where the click was...
            adjust: {mouse: false} // ...but don't follow the mouse
        };
        this.content = {text: ""};
        this.show = false; // Do not show on mouseover of the graph
    },
    addOnHoverLabeller: function (div_id, elements, labeller) {
        var config = new tremppi.qtip.getConfig();
        var api = $("#" + div_id).qtip(config).qtip('api');
        elements.on("tapdragover", function (eve) {
            var my_data = eve.cyTarget.data();
            api.set("content.text", labeller(my_data));
            api.show();
        });
        elements.on("tapdragout", function () {
            api.hide();
        });
    }
};
tremppi.editablegrid = {
    // test is a row is not empty
    isEmpty: function (columns) {
        for (var i = 0; i < columns.length; i++) {
            if (columns[i] !== "" && columns[i] !== false)
                return false;
        }
        return true;
    }
};
tremppi.cytoscape = {
    // test if nodes all have positions
    hasAllPositions: function (nodes) {
        if (typeof nodes === 'undefined')
            return true;
        for (var i = 0; i < nodes.length; i++) {
            if (!nodes[i].position)
                return false;
            if (!nodes[i].position.x || !nodes[i].position.y)
                return false;
        }
        return true;
    }
};
tremppi.report = {
    selections: ["left", "mid", "right", "all"],
    addSetup: function (setup) {
        values = ["date", "name", "pool_size", "select", "selected", "compare", "compared"];
        for (var i = 0; i < values.length; i++) {
            $("#analysis_setup").append('<div class="decription"><span class="desc_title">' +
                    values[i] +
                    ':</span> <span class="desc_content" id="analysis_date">' +
                    setup[values[i]] +
                    '</span></div>');
        }
    }, // Synchronization in between the graphs
    synchronize: function (config, labelFunction) {
        var cys = [];
        for (var id = 0; id < config.types.length; id++) {
            cys[id] = $('#graph_' + config.types[id]).cytoscape('get');
        }
        var nodes = cys[0].elements("node");

        // Sets all nodes with the id to the position given by graph
        var moveFunction = function (graph, id) {
            return function (evt) {
                for (var i = 0; i < config.types.length; i++) {
                    cys[i].$(id).renderedPosition(graph.$(id).renderedPosition());
                    tremppi.data[config.types[i]]["elements"] = cys[i].json().elements;
                }
                tremppi.common.save();
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
                labelFunction();
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
    }
};