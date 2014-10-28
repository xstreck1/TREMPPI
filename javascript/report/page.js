tremppi_report.Page = {
    loadData: function(config) {
        // Set the types used
        config = {};
        if (report_data.setup.comparative) {
            config.types = ['select', 'differ', 'compare'];
        } else {
            config.types = ['select'];
        }

        // load and display graphs
        var loadGraph = function(config, type)
        {
            var name = "reg_graph_" + type;
            var graph = report_data.Regulatory[type];
            tremppi_report.Helpers.configure(config, graph.elements, type);
            tremppi_report.Graph.makeGraph(graph.elements, type);
            $('#' + name).cytoscape('get').load(graph.elements);
        };
        for (var i = 0; i < config.types.length; i++) {
            loadGraph(config, config.types[i]);
        }
        var bg_color = '#888888';
        tremppi_report.Helpers.assignPositions(config, "reg", "graph");
        tremppi_report.Helpers.deactivateUnused(config, "reg", "graph");
        $('#regulatory_container').css('padding-bottom', '30%');
        for (var i = 0; i < config.types.length; i++) {
            $('#reg_graph_' + config.types[i]).css('background-color', bg_color);
        }

        // compute and display lables
        tremppi_report.Regulatory.relative = false;
        tremppi_report.Graph.labelSwitch(config, tremppi_report.Regulatory.relative);
        tremppi_report.Helpers.assignPositions(config, "reg", "label");
        tremppi_report.Helpers.deactivateUnused(config, "reg", "label");
        $('#regulatory_labels').css('padding-bottom', '45px');
        labelRefresh = function() {
            tremppi_report.Labels.loadLabels(config, tremppi_report.Regulatory.relative);
        };
        $(window).resize(labelRefresh);
        tremppi_report.Graph.synchronize(config, labelRefresh);

        // display captions
        tremppi_report.Helpers.assignPositions(config, "reg", "caption");
        tremppi_report.Helpers.deactivateUnused(config, "reg", "caption");

        // Create the control buttons functions
        $("#relative_button").click(function() {
            tremppi_report.Regulatory.relative = !tremppi_report.Regulatory.relative;
            tremppi_report.Graph.labelSwitch(config, tremppi_report.Regulatory.relative);
            tremppi_report.Labels.loadLabels(config, tremppi_report.Regulatory.relative);
            this.innerHTML = tremppi_report.Regulatory.relative ? "absolute" : "relative";
        });
        var resizeViews = function(change) {
            var height = $('#regulatory_container').css('padding-bottom');
            height = height.replace(new RegExp('[^\\d.]', "g"), '');
            height = (height - change) + "px";
            $('#regulatory_container').css('padding-bottom', height);
            for (var i = 0; i < config.types.length; i++) {
                $('#reg_graph_' + config.types[i]).cytoscape('get').resize();
            }
        };

        $("#copy_graph").click(function() {
            for (var i = 0; i < config.types.length; i++) {
                var cy = $('#reg_graph_' + config.types[i]).cytoscape('get');
                report_data.Regulatory[config.types[i]] = cy.json();
            }
            tremppi_common.promptWithContent("report_data");
        });

        $("#regulatory_description").html((
                (report_data.setup.comparative) ? tremppi_report.Values.reg_caption_c : tremppi_report.Values.reg_caption_nonc)
                + tremppi_report.Values.reg_caption_common);

        // Replace text content
        var setup = report_data.setup;
        $("title").html(setup.name + " statistical analysis report");
        $("#analysis_date").html(setup.date);
        $("#model_name").html(setup.name);
        $("#pool_size").html(setup.pool_size);
        var select_stmt = setup.select_statement ? setup.select_statement : "all";
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
                .bind('dragstop', function(event) {
                    resizeViews(regulatory_y - $("#regulatory_dragger").offset().top);
                    $("#regulatory_dragger").css("top", "0px").css("left", "0px");
                    regulatory_y = $("#regulatory_dragger").offset().top;
                }).hover(function() {
            $(this).css('cursor', 'move');
        }, function() {
            $(this).css('cursor', 'auto');
        });

    }
};
