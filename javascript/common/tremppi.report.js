/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.report = {
    selections : ["left", "mid", "right", "all"],
    addSetup : function(setup) {
        values = ["date", "name", "pool_size", "select", "selected", "compare", "compared" ];
        for (var i = 0; i < values.length; i++) {
            $("#analysis_setup").append('<div class="decription"><span class="desc_title">' + 
                    values[i] + 
                    ':</span> <span class="desc_content" id="analysis_date">' + 
                    setup[values[i]] + 
                    '</span></div>');
        }
    },    // Synchronization in between the graphs
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
    },
};