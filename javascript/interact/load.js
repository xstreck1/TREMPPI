tremppi.interact.Values = {
    caption_c: "a) statistical properties of the selected parametrizations set, c) properties of the compared parametrizations set, b) difference between the two. "
            + "F stands for frequency of occurrence of an edge in the given set, P for the Pearson correlation coefficient between the value of the regulator and the parameter value. "
            + "Blunt edges denote positive and arrows describe the negative correlation value in the graphs a) and c). "
            + "In the graph b) the positive change in frequency is depicted by full edge, the negative by the dashed one. "
            + "The dotted edge is used when the difference in frequency is zero.",
    caption_nonc: "a) statistical properties of the selected parametrizations set. "
            + "F stands for frequency of occurrence of an edge in the given set, P for the Pearson correlation coefficient between the value of the regulator and the parameter value. "
            + "Blunt edges denote positive and arrows describe the negative correlation value. "
};

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
    tremppi.interact.Helpers.deactivateUnused("graph");
    $('#graph_container').css('height', '600px');
    for (var i = 0; i < config.types.length; i++) {
        $('#graph_' + config.types[i]).css('background-color', bg_color);
    }

    // compute and display lables
    tremppi.data.relative = false;
    tremppi.data.weighted = false;
    tremppi.interact.Graph.labelSwitch(config, tremppi.data.relative, tremppi.data.weighted);
    tremppi.interact.Helpers.deactivateUnused("label");
    $('#graph_labels').css('padding-bottom', '45px');
    labelRefresh = function () {
        tremppi.interact.Labels.loadLabels(config, tremppi.data.relative);
    };
    $(window).resize(labelRefresh);
    tremppi.interact.Graph.synchronize(config, labelRefresh);

    // display captions
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
};
