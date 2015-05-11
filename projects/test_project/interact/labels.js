/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.interact.Labels = {
    // Adds reactive tip window that appears on mouseover on the edge
    addQtip: function (cys, type) {
        var num_of_decimals = 3;

        var edges = cys.filter("edge");
        var labeller = function (my_data) {
            return "source: " + my_data.source + "<br />"
                    + "target: " + my_data.target + "<br />"
                    + "pearson: " + my_data.Pearson.toFixed(num_of_decimals) + "<br />"
                    + "frequency: " + my_data.Frequency.toFixed(num_of_decimals) + "<br />"
                    + "expected frequency: " + my_data.ExpectedFreq.toFixed(num_of_decimals) + "<br />";
        };

        tremppi.qtip.addOnHoverLabeller("graph_" + type, edges, labeller);
    },
    loadLabels: function () {
        var config = tremppi.data.config;
        
        var bar_left = 110;
        var num_of_decimals = 3;
        var rel_string = config.relative ? "relative" : "absolute";
        var width_type = config.weighted ? "weight" : "width";

        var cys = [];
        var papers = [];
        for (var id = 0; id < config.types.length; id++) {
            cys[id] = $('#graph_' + config.types[id]).cytoscape('get');
            papers[id] = new paper.PaperScope();
            var legend_height = Math.min(config[config.types[id]][width_type].max * cys[id].zoom() + 40, $("#data").height() / 3);
            $('#legend_' + config.types[id]).height(legend_height);
            papers[id].setup($('#legend_' + config.types[id])[0]);
        }
        var F_height = 40;
        var P_height = 20;

        var makeText = function (content, position) {
            var text = new paper.PointText(position);
            text.fillColor = 'black';
            text.fontSize = 20;
            text.fontFamily = 'Courier New';
            text.content = content;
            return text;
        };

        var addEdgeWidth = function (type, paper, width_ratio) {
            paper.activate();
            var bar_height = F_height - 10;
            var bar_right = paper.view.viewSize.width - 70;

            // Create the bar
            var bar = new paper.Path();
            bar.fillColor = 'black';
            bar.strokeWidth = 0;
            var min_width = config[type][width_type].min * width_ratio;
            var max_width = config[type][width_type].max * width_ratio;
            bar.add(new paper.Point(bar_right, bar_height));
            bar.add(new paper.Point(bar_left, bar_height + (max_width - min_width) / 2));
            bar.add(new paper.Point(bar_left, bar_height + (max_width + min_width) / 2));
            bar.add(new paper.Point(bar_right, bar_height + max_width));
            bar.add(new paper.Point(bar_right, bar_height));
            // Add the label
            var min = config[type][rel_string][width_type].min.toFixed(num_of_decimals);
            min = min >= 0 ? ' ' + min : min;
            var max = config[type][rel_string][width_type].max.toFixed(num_of_decimals);
            var F_pad = (config[type][width_type].max * width_ratio) / 2 - 4;
            makeText('F: ', new paper.Point(10, F_height + F_pad));
            makeText(min, new paper.Point(bar_left - 75, F_height + F_pad));
            makeText(max, new paper.Point(bar_right + 5, F_height + F_pad));
            paper.view.draw();
        };

        var addGradient = function (type, paper, id) {
            paper.activate();
            var bar_right = paper.view.viewSize.width - 70;
            makeText('P: ', new paper.Point(10, P_height));
            // Make the bar   
            var bar = new paper.Path.Rectangle(
                    new paper.Rectangle(
                            new paper.Point(bar_left, P_height - 10),
                            new paper.Point(bar_right, P_height)));
            bar.fillColor = {gradient: {}};
            bar.fillColor.origin = [bar_left, 0];
            bar.fillColor.destination = [bar_right, 0];
            if (id === 0)
                bar.fillColor.gradient.stops = ['yellow', 'green'];
            else if (id === 1)
                bar.fillColor.gradient.stops = ['red', 'yellow', 'green'];
            else if (id === 2)
                bar.fillColor.gradient.stops = ['yellow', 'red'];
            bar.strokeColor = 'black';
            bar.strokeWidth = 1;
            // Make the text
            var min = config[type][rel_string].color.min.toFixed(num_of_decimals);
            min = min >= 0 ? ' ' + min : min;
            var max = config[type][rel_string].color.max.toFixed(num_of_decimals);
            makeText(min, new paper.Point(bar_left - 75, P_height));
            makeText(max, new paper.Point(bar_right + 5, P_height));

            paper.view.draw();
        };

        // Assign the papers
        for (var id = 0; id < config.types.length; id++) {
            addEdgeWidth(config.types[id], papers[id], cys[id].zoom());
            addGradient(config.types[id], papers[id], id);
        }
    }
};
