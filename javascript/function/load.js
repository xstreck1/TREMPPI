tremppi.function.Values = {
    caption_c: "caption_c",
    caption_nonc: "caption_nonc"
};

tremppi.function.load = function () {

    var default_config = {
        types: [],
        relative: false,
        weigthed: false,
        select: {
            absolute: {
                width: {min: 0, max: 1},
                color: {min: 0, max: 1},
                bias: {min: 0, max: 1}
            },
            relative: {},
            width: {min: 1, max: 10},
            weight: {min: 1, max: 10},
            color: {min: "yellow", max: "green"},
            bias: {min: "black", max: "white"}
        },
        differ: {
            absolute: {
                width: {min: 0, max: 1},
                color: {min: -1, max: 1},
                bias: {min: -1, max: 1}
            },
            relative: {},
            width: {min: 1, max: 10},
            weight: {min: 1, max: 10},
            color_neg: {min: "yellow", max: "red"},
            color_pos: {min: "yellow", max: "green"},
            bias: {min: "black", max: "white"}
        },
        compare: {
            absolute: {
                width: {min: 0, max: 1},
                color: {min: 0, max: 1},
                bias: {min: 0, max: 1}
            },
            relative: {},
            width: {min: 1, max: 10},
            weight: {min: 1, max: 10},
            color: {min: "yellow", max: "red"},
            bias: {min: "black", max: "white"}
        },
        setup: true
    };

    var getBound = function (edges, param, fun, signed, weighted) {
        if (signed && fun === "min")
            return 0;

        var result = fun === "min" ? Number.MAX_VALUE : Number.MIN_VALUE;

        var min_func = signed ? function (x, y) {
            var res = -1 * Math.max(Math.abs(x), Math.abs(y));
            return res;
        } : function (x, y) {
            var res = Math.min(Math.abs(x), Math.abs(y));
            return res;
        };
        var max_func = signed ? function (x, y) {
            return Math.max(Math.abs(x), Math.abs(y));
        } : function (x, y) {
            var res = Math.max(Math.abs(x), Math.abs(y));
            return res;
        };

        var cmp_function = fun === 'min' ? min_func : max_func;
        for (var edge_no = 0; edge_no < edges.length; edge_no++) {
            var value = edges[edge_no].data[param];
            if (weighted)
                value /= edges[edge_no].data.ExpectedFreq;
            result = cmp_function(result, value);
        }
        return result;
    };
    // gives the ranges of values and their respective mapping
    var configure = function (config, graph, type) {
        if (typeof graph.edges === 'undefined')
            graph.edges = [];
        config[type].relative = {
            color: {
                min: getBound(graph.edges, "Pearson", "min", false, false),
                max: getBound(graph.edges, "Pearson", "max", false, false)
            }, 
            bias: {
                min: getBound(graph.nodes, "Bias", "min", false, false),
                max: getBound(graph.nodes, "Bias", "max", false, false)
            }
        };
    };
    // Re-layout if only one graph is selected
    var selectClick = function (type) {
        return function () {
            for (var j = 0; j < config.types.length; j++) {
                if (type === config.types[j]) {
                    $("#container_" + config.types[j]).css("display", "block").css("left", "0").css("width", "100%");
                    $("#graph_" + config.types[j]).cytoscape('get').resize();
                } else {
                    $("#container_" + config.types[j]).css("display", "none");
                }
            }
            tremppi.function.Labels.loadLabels();
        };
    };
    var selectAll = function () {
        for (var j = 0; j < config.types.length; j++) {
            $("#container_" + config.types[j]).removeAttr('style');
            $("#graph_" + config.types[j]).cytoscape('get').resize();
        }
        tremppi.function.Labels.loadLabels();
    };
    var selectSelect = function () {
        $("#container_differ").css("display", "none");
        $("#container_compare").css("display", "none");
        $("#container_select").css("display", "block").css("left", "0").css("width", "100%");
        $("#graph_select").cytoscape('get').resize();
    };

    // load and display graphs
    var loadGraph = function (config, type)
    {
        var name = "graph_" + type;
        var graph = tremppi.data[type];
        configure(config, graph.elements, type);
        tremppi.function.Graph.makeGraph(graph.elements, type);
    };

    var setButtonLabels = function (config) {
        this.innerHTML = tremppi.data.config.relative ? "absolute" : "relative";

    };

    // Set the types used
    if (typeof tremppi.data.config === 'undefined') {
        var config = {
            config: default_config
        };
        $.extend(tremppi.data, config);
    }
    var config = tremppi.data.config;
    config.types = tremppi.data.setup.comparative ? ['select', 'differ', 'compare'] : ['select'];

    // Add setup data
    tremppi.report.addSetup(tremppi.data.setup);

    for (var i = 0; i < config.types.length; i++)
        loadGraph(config, config.types[i]);
    tremppi.report.synchronize(config, tremppi.function.Labels.loadLabels);

    if (!tremppi.data.setup.comparative)
        selectSelect();

    // compute and display lables
    tremppi.function.Graph.applyVisuals(config);
    $('#graph_labels').css('padding-bottom', '45px');

    // display captions
    $("#description").html((
            (tremppi.data.setup.comparative) ? tremppi.function.Values.caption_c : tremppi.function.Values.caption_nonc)
            + tremppi.function.Values.caption_common);

    // Create the control buttons functions
    $("#setup_button").click(function () {
        config.setup = !config.setup;
        $("#analysis_setup").css("display", config.setup ? "block" : "none");
        this.innerHTML = config.setup ? "Hide" : "Show";
    });
    if (!config.setup) {
        $("#analysis_setup").css("display", "none");
        $("#setup_button").html("Show");
    }
    $("#relative_button").click(function () {
        config.relative = !config.relative;
        tremppi.function.Graph.applyVisuals(config);
        tremppi.function.Labels.loadLabels(config);
        this.innerHTML = config.relative ? "absolute" : "relative";
    });
    if (config.relative) {
        $("#relative_button").html("absolute");
    }
    if (tremppi.data.setup.comparative) {
        for (var i = 0; i < config.types.length; i++)
            $("#button_" + config.types[i]).click(selectClick(config.types[i]));
        $("#button_all").click(selectAll);
    } else {
        $("#selection_buttons").html("");
    }
    $("#function_controls").click(tremppi.common.save);
};
