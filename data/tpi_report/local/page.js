/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tpi_report.Page = {
    loadData: function(config) {
        // Set the types used
        config = {};
        if (tpi_report.setup.comparative) {
            config.types = ['select', 'differ', 'compare'];
        } else {
            config.types = ['select'];
        }

        // load and display graphs
        var loadGraph = function(config, type)
        {
            var name = "reg_graph_" + type;
            var graph = tpi_report.Regulatory[type];
            tpi_report.Helpers.configure(config, graph.elements, type);
            tpi_report.Graph.makeGraph(graph.elements, type);
            $('#' + name).cytoscape('get').load(graph.elements);
        };
        for (var i = 0; i < config.types.length; i++) {
            loadGraph(config, config.types[i]);
        }
        var bg_color = '#888888';
        tpi_report.Helpers.assignPositions(config, "reg", "graph");
        tpi_report.Helpers.deactivateUnused(config, "reg", "graph");
        $('#regulatory_container').css('padding-bottom', '50%');
        for (var i = 0; i < config.types.length; i++) {
            $('#reg_graph_' + config.types[i]).css('border-style', 'solid').css('border-width', '1').css('background-color', bg_color).css('padding-left', '1');
        }

        // compute and display lables
        tpi_report.Regulatory.relative = false;
        tpi_report.Graph.labelSwitch(config, tpi_report.Regulatory.relative);
        tpi_report.Helpers.assignPositions(config, "reg", "label");
        tpi_report.Helpers.deactivateUnused(config, "reg", "label");
        $('#regulatory_labels').css('padding-bottom', '45px');
        labelRefresh = function() {
            tpi_report.Labels.loadLabels(config, tpi_report.Regulatory.relative);
        };
        $(window).resize(labelRefresh);
        tpi_report.Graph.synchronize(config, labelRefresh);

        // display captions
        tpi_report.Helpers.assignPositions(config, "reg", "caption");
        tpi_report.Helpers.deactivateUnused(config, "reg", "caption");
        $('#regulatory_caption').css('padding-bottom', '40px');

        // Create the control buttons functions
        $("#relative_button").click(function() {
            tpi_report.Regulatory.relative = !tpi_report.Regulatory.relative;
            tpi_report.Graph.labelSwitch(config, tpi_report.Regulatory.relative);
            tpi_report.Labels.loadLabels(config, tpi_report.Regulatory.relative);
            this.innerHTML = tpi_report.Regulatory.relative ? "Absolute" : "Relative";
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
        $("#reset_button").click(function() {
            if (tpi_report.setup.comparative) {
                $('#reg_graph_differ').cytoscape('get').reset();
                $('#reg_graph_compare').cytoscape('get').reset();
            }
            $('#reg_graph_select').cytoscape('get').reset();
        });

        $("#regulatory_description").html(((tpi_report.setup.comparative) ? tpi_report.Values.reg_caption_c : tpi_report.Values.reg_caption_nonc) + tpi_report.Values.reg_caption_common);

        // Replace text content
        var setup = tpi_report.setup;
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
