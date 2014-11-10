/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi_report.Helpers = {
    getBound: function(edges, param, fun, signed) {
        if (signed && fun === "min")
            return 0;
        
        var result = fun === "min" ? Number.MAX_VALUE : Number.MIN_VALUE;

        var min_func = signed ? function(x, y) {
            var res = -1 * Math.max(Math.abs(x), Math.abs(y));
            return res;
        } : function(x, y) {
            var res = Math.min(Math.abs(x), Math.abs(y));
            return res;
        };
        var max_func = signed ? function(x, y) {
            return Math.max(Math.abs(x), Math.abs(y));
        } : function(x, y) {
            var res = Math.max(Math.abs(x), Math.abs(y));
            return res;
        };

        var cmp_function = fun === 'min' ? min_func : max_func;
        for (var edge_no = 0; edge_no < edges.length; edge_no++) {
            result = cmp_function(result, edges[edge_no].data[param]);
        }
        return result;
    },
    
    
    // gives the ranges of values and their respective mapping
    configure: function(config, graph, type) {
        config.my_val = true;
        if (type === "select")
            config[type] = {
                absolute: {width: {min: 0, max: 1}, color: {min: 0, max: 1}},
                relative: {},
                width: {min: 1, max: 10},
                color: {min: "yellow", max: "green"}
            };
        if (type === "differ")
            config[type] = {
                absolute: {width: {min: 0, max: 1}, color: {min: 0, max: 1}},
                relative: {},
                width: {min: 1, max: 10},
                color_neg: {min: "yellow", max: "red"},
                color_pos: {min: "yellow", max: "green"}
            };
        if (type === "compare")
            config[type] = {
                absolute: {width: {min: 0, max: 1}, color: {min: 0, max: 1}},
                relative: {},
                width: {min: 1, max: 10},
                color: {min: "yellow", max: "red"}
            };
        config[type].relative = {
            width: {
                min: this.getBound(graph.edges, "Frequency", "min", false),
                max: this.getBound(graph.edges, "Frequency", "max", false)},
            color: {
                min: this.getBound(graph.edges, "Pearson", "min", type === "differ"),
                max: this.getBound(graph.edges, "Pearson", "max", type === "differ")}};
    },
    assignPositions: function(config, analysis, tag) {
        if (report_data.setup.comparative) {
            $('#' + analysis + '_' + tag + '_select').css('width', '33%').css('left', '0%');
            $('#' + analysis + '_' + tag + '_differ').css('width', '33%').css('left', '33.5%');
            $('#' + analysis + '_' + tag + '_compare').css('width', '33%').css('left', '67%');
        } else {
            $('#' + analysis + '_' + tag + '_select').css('width', '50%').css('left', '25%');
        }
        ;
    },
    deactivateUnused: function(config, analysis, tag) {
        if (!report_data.setup.comparative) {
            $('#' + analysis + '_' + tag + '_differ').html("");
            $('#' + analysis + '_' + tag + '_compare').html("");
        }
        ;
    }
};

