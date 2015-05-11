/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi */

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
tremppi.w2ui = {
    changeFunction: function (columns, records) {
        return function (event) {
            event.onComplete = function (event) {
                var column_name = columns[event.column].field;
                records[event.index][column_name] = event.value_new;
                records[event.index].changes = {};
                tremppi.widget.save();
            }
        };
    },
    findByRecID: function (array, recid) {
        return array[iByRecID(array, recid)];
    },
    iByRecID: function (array, recid) {
        for (var i = 0; i < array.length; i++) {
            if (array[i].recid === recid)
                return i;
        }
        console.log("Error. Have not found element " + recid);
    },
    getFreeRecID: function (records) {
        var ids = records.map(function (entry) {
            return parseInt(entry.recid);
        });
        ids.sort(function (a, b) {
            return a - b;
        });
        for (var id = 0; id < ids.length; id++) {
            if (ids[id] !== id) {
                return id;
            }
        }
        return id;
    },
    checkAll: function (event, records, grid) {
        var new_val = !event.originalEvent.item.checked;
        records.forEach(function (record) {
            record[event.target] = new_val;
        });
        grid.records = records;
    },
    deleteSelected: function (records, grid) {
        grid.getSelection().forEach(function (recid) {
            for (var i = 0; i < records.length; i++) {
                if (records[i].recid === recid) {
                    records.splice(i, 1);
                    break;
                }
            }
        });
        grid.records = records;
    },
    duplicateSelected: function (records, grid) {
        var new_records = [];
        grid.getSelection().forEach(function (recid) {
            for (var i = 0; i < records.length; i++) {
                if (records[i].recid === recid) {
                    var new_entry = {};
                    $.extend(true, new_entry, records[i]);
                    new_entry.recid = tremppi.w2ui.getFreeRecID(records);
                    if (typeof new_entry.name !== 'undefined') {
                        new_entry.name += " (copy)";
                    }
                    new_records.push(new_entry.recid);
                    records.splice(i + 1, 0, new_entry);
                    break;
                }
            }
        });
        grid.records = records;
    },
    up: function (grid, records) {
        var selection = grid.getSelection();
        if (selection.length === 1) {
            var recid = grid.getSelection()[0];
            var i = tremppi.w2ui.iByRecID(records, recid);
            if (i > 0) {
                var temp = records[i];
                records[i] = records[i - 1];
                records[i - 1] = temp;
            }
        }
    },
    down: function (grid, records) {
        var selection = grid.getSelection();
        if (selection.length === 1) {
            var recid = grid.getSelection()[0];
            var i = tremppi.w2ui.iByRecID(records, recid);
            if (i + 1 < records.length) {
                var temp = records[i];
                records[i] = records[i + 1];
                records[i + 1] = temp;
            }
        }
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
tremppi.log = function (content, level) {
    if (typeof level === "undefined")
        level = "info";

    var date = new Date();
    $("#log_line").html("[" + date.getHours() + ":" + date.getMinutes() + ":" + date.getSeconds() + "] " + content);
    if ($("#log_line").length > 0)
        $("#log_line")[0].className = level;
};