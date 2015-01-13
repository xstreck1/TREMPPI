tremppi.report.load = function () {
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
        var name = "reg_graph_" + type;
        var graph = tremppi.data.Regulatory[type];
        tremppi.report.Helpers.configure(config, graph.elements, type);
        tremppi.report.Graph.makeGraph(graph.elements, type);
    };
    for (var i = 0; i < config.types.length; i++) {
        loadGraph(config, config.types[i]);
    }
    var bg_color = '#888888';
    tremppi.report.Helpers.assignPositions(config, "reg", "graph");
    tremppi.report.Helpers.deactivateUnused(config, "reg", "graph");
    $('#regulatory_container').css('padding-bottom', '30%');
    for (var i = 0; i < config.types.length; i++) {
        $('#reg_graph_' + config.types[i]).css('background-color', bg_color);
    }

    // compute and display lables
    tremppi.report.Regulatory.relative = false;
    tremppi.report.Regulatory.weighted = false;
    tremppi.report.Graph.labelSwitch(config, tremppi.report.Regulatory.relative, tremppi.report.Regulatory.weighted);
    tremppi.report.Helpers.assignPositions(config, "reg", "label");
    tremppi.report.Helpers.deactivateUnused(config, "reg", "label");
    $('#regulatory_labels').css('padding-bottom', '45px');
    labelRefresh = function () {
        tremppi.report.Labels.loadLabels(config, tremppi.report.Regulatory.relative);
    };
    $(window).resize(labelRefresh);
    tremppi.report.Graph.synchronize(config, labelRefresh);

    // display captions
    tremppi.report.Helpers.assignPositions(config, "reg", "caption");
    tremppi.report.Helpers.deactivateUnused(config, "reg", "caption");

    // Create the control buttons functions
    $("#relative_button").click(function () {
        tremppi.report.Regulatory.relative = !tremppi.report.Regulatory.relative;
        tremppi.report.Graph.labelSwitch(config, tremppi.report.Regulatory.relative, tremppi.report.Regulatory.weighted);
        tremppi.report.Labels.loadLabels(config, tremppi.report.Regulatory.relative, tremppi.report.Regulatory.weighted);
        this.innerHTML = tremppi.report.Regulatory.relative ? "absolute" : "relative";
    });
    $("#weighted_button").click(function () {
        tremppi.report.Regulatory.weighted = !tremppi.report.Regulatory.weighted;
        tremppi.report.Graph.labelSwitch(config, tremppi.report.Regulatory.relative, tremppi.report.Regulatory.weighted);
        tremppi.report.Labels.loadLabels(config, tremppi.report.Regulatory.relative, tremppi.report.Regulatory.weighted);
        this.innerHTML = tremppi.report.Regulatory.weighted ? "total" : "weighted";
    });
    var resizeViews = function (change) {
        var height = $('#regulatory_container').css('padding-bottom');
        height = height.replace(new RegExp('[^\\d.]', "g"), '');
        height = (height - change) + "px";
        $('#regulatory_container').css('padding-bottom', height);
        for (var i = 0; i < config.types.length; i++) {
            $('#reg_graph_' + config.types[i]).cytoscape('get').resize();
        }
    };

    $("#copy_graph").click(function () {
        for (var i = 0; i < config.types.length; i++) {
            var cy = $('#reg_graph_' + config.types[i]).cytoscape('get');
            tremppi.data.Regulatory[config.types[i]] = cy.json();
        }
        tremppi.common.promptWithContent("report");
    });

    $("#regulatory_description").html((
            (tremppi.data.setup.comparative) ? tremppi.report.Values.reg_caption_c : tremppi.report.Values.reg_caption_nonc)
            + tremppi.report.Values.reg_caption_common);

    // Replace text content
    var setup = tremppi.data.setup;
    $("title").html(setup.name + " statistical analysis report");
    $("#analysis_date").html(setup.date);
    $("#model_name").html(setup.name);
    $("#pool_size").html(setup.pool_size);
    var select_stmt = setup.select ? setup.select : "all";
    $("#select_statement").html(select_stmt);
    $("#selected").html(setup.selected + ' ' + '(' + (setup.selected / setup.pool_size * 100).toFixed(2) + '%)');
    var compare_stmt = setup.compare ? setup.compare : "all";
    compare_stmt = setup.comparative ? compare_stmt : "Not comparing";
    $("#compare_statement").html(compare_stmt);
    $("#compared").html(setup.compared + ' ' + '(' + (setup.compared / setup.pool_size * 100).toFixed(2) + '%)');

    // Create the resize dragger
    var regulatory_y = $("#regulatory_dragger").offset().top;
    $("#regulatory_dragger")
            .draggable({axis: "y"})
            .bind('dragstop', function (event) {
                resizeViews(regulatory_y - $("#regulatory_dragger").offset().top);
                $("#regulatory_dragger").css("top", "0px").css("left", "0px");
                regulatory_y = $("#regulatory_dragger").offset().top;
            }).hover(function () {
        $(this).css('cursor', 'move');
    }, function () {
        $(this).css('cursor', 'auto');
    });

};