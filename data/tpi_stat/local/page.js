/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tpi_stat.Page = {
    loadData: function(config) {
        // Set the types used
        config = {};
        if (tpi_stat.setup.comparative) {
            config.types = ['select', 'differ', 'compare'];
        } else {
            config.types = ['select'];
        }

        // load and display graphs
        var loadGraph = function(config, type)
        {
            var name = "reg_graph_" + type;
            var graph = tpi_stat.Regulatory[type];
            tpi_stat.Helpers.configure(config, graph.elements, type);
            tpi_stat.Graph.makeGraph(graph.elements, type);
            $('#' + name).cytoscape('get').load(graph.elements);
        };
        for (var i = 0; i < config.types.length; i++) {
            loadGraph(config, config.types[i]);
        }
        var bg_color = '#888888';
        tpi_stat.Helpers.assignPositions(config, "reg", "graph");
        tpi_stat.Helpers.deactivateUnused(config, "reg", "graph");
        $('#regulatory_container').css('padding-bottom', '50%');
        for (var i = 0; i < config.types.length; i++) {
            $('#reg_graph_' + config.types[i]).css('border-style', 'solid').css('border-width', '1').css('background-color', bg_color).css('padding-left', '1');
        }

        // compute and display lables
        tpi_stat.Regulatory.relative = false;
        tpi_stat.Graph.labelSwitch(config, tpi_stat.Regulatory.relative);
        tpi_stat.Helpers.assignPositions(config, "reg", "label");
        tpi_stat.Helpers.deactivateUnused(config, "reg", "label");
        $('#regulatory_labels').css('padding-bottom', '45px');
        labelRefresh = function() {
            tpi_stat.Labels.loadLabels(config, tpi_stat.Regulatory.relative);
        };
        $(window).resize(labelRefresh);
        tpi_stat.Graph.synchronize(config, labelRefresh);

        // display captions
        tpi_stat.Helpers.assignPositions(config, "reg", "caption");
        tpi_stat.Helpers.deactivateUnused(config, "reg", "caption");
        $('#regulatory_caption').css('padding-bottom', '40px');

        // Create the control buttons functions
        $("#relative_button").click(function() {
            tpi_stat.Regulatory.relative = !tpi_stat.Regulatory.relative;
            tpi_stat.Graph.labelSwitch(config, tpi_stat.Regulatory.relative);
            tpi_stat.Labels.loadLabels(config, tpi_stat.Regulatory.relative);
            this.innerHTML = tpi_stat.Regulatory.relative ? "Absolute" : "Relative";
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
            if (tpi_stat.setup.comparative) {
                $('#reg_graph_differ').cytoscape('get').reset();
                $('#reg_graph_compare').cytoscape('get').reset();
            }
            $('#reg_graph_select').cytoscape('get').reset();
        });

        $("#regulatory_description").html(((tpi_stat.setup.comparative) ? tpi_stat.Values.reg_caption_c : tpi_stat.Values.reg_caption_nonc) + tpi_stat.Values.reg_caption_common);

        // Replace text content
        var setup = tpi_stat.setup;
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
