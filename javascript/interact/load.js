tremppi.interact.load = function () {
    // Set the types used
    config = {};
    if (tremppi.data.setup.comparative) {
        config.types = ['select', 'differ', 'compare'];
    } else {
        config.types = ['select'];
    }

    // load and display graphs
    var loadGraph = function (config, type)
    {
        var name = "graph_" + type;
        var graph = tremppi.data[type];
        tremppi.interact.Helpers.configure(config, graph.elements, type);
        tremppi.interact.Graph.makeGraph(graph.elements, type);
    };
    for (var i = 0; i < config.types.length; i++) {
        loadGraph(config, config.types[i]);
    }
    var bg_color = '#888888';
    tremppi.interact.Helpers.assignPositions("graph");
    tremppi.interact.Helpers.deactivateUnused("graph");
    $('#graph_container').css('height', '600px');
    for (var i = 0; i < config.types.length; i++) {
        $('#graph_' + config.types[i]).css('background-color', bg_color);
    }

    // compute and display lables
    tremppi.data.relative = false;
    tremppi.data.weighted = false;
    tremppi.interact.Graph.labelSwitch(config, tremppi.data.relative, tremppi.data.weighted);
    tremppi.interact.Helpers.assignPositions("label");
    tremppi.interact.Helpers.deactivateUnused("label");
    $('#graph_labels').css('padding-bottom', '45px');
    labelRefresh = function () {
        tremppi.interact.Labels.loadLabels(config, tremppi.data.relative);
    };
    $(window).resize(labelRefresh);
    tremppi.interact.Graph.synchronize(config, labelRefresh);

    // display captions
    tremppi.interact.Helpers.assignPositions("caption");
    tremppi.interact.Helpers.deactivateUnused("caption");

    // Create the control buttons functions
    $("#relative_button").click(function () {
        tremppi.data.relative = !tremppi.data.relative;
        tremppi.interact.Graph.labelSwitch(config, tremppi.data.relative, tremppi.data.weighted);
        tremppi.interact.Labels.loadLabels(config, tremppi.data.relative, tremppi.data.weighted);
        this.innerHTML = tremppi.data.relative ? "absolute" : "relative";
    });
    $("#weighted_button").click(function () {
        tremppi.data.weighted = !tremppi.data.weighted;
        tremppi.interact.Graph.labelSwitch(config, tremppi.data.relative, tremppi.data.weighted);
        tremppi.interact.Labels.loadLabels(config, tremppi.data.relative, tremppi.data.weighted);
        this.innerHTML = tremppi.data.weighted ? "total" : "weighted";
    });
    var resizeViews = function (change) {
        var height = $('#graph_container').css('padding-bottom');
        height = height.replace(new RegExp('[^\\d.]', "g"), '');
        height = (height - change) + "px";
        $('#graph_container').css('padding-bottom', height);
        for (var i = 0; i < config.types.length; i++) {
            $('#graph_' + config.types[i]).cytoscape('get').resize();
        }
    };

    $("#copy_graph").click(function () {
        for (var i = 0; i < config.types.length; i++) {
            var cy = $('#graph_' + config.types[i]).cytoscape('get');
            tremppi.data[config.types[i]] = cy.json();
        }
        tremppi.common.promptWithContent("report");
    });

    $("#description").html((
            (tremppi.data.setup.comparative) ? tremppi.interact.Values.caption_c : tremppi.interact.Values.caption_nonc)
            + tremppi.interact.Values.caption_common);

    // Add setup data
    tremppi.report.addSetup(tremppi.data.setup);

    // Create the resize dragger
    var y_pos = $("#dragger").offset().top;
    $("#dragger")
            .draggable({axis: "y"})
            .bind('dragstop', function (event) {
                resizeViews(y_pos - $("#dragger").offset().top);
                $("#dragger").css("top", "0px").css("left", "0px");
                y_pos = $("#dragger").offset().top;
            }).hover(function () {
        $(this).css('cursor', 'move');
    }, function () {
        $(this).css('cursor', 'auto');
    });

};
