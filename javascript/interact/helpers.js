/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.interact.Helpers = {
    getBound: function (edges, param, fun, signed, weighted) {
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
    },
    // gives the ranges of values and their respective mapping
    configure: function (config, graph, type) {
        config.my_val = true;
        config[type] = {
            absolute: {
                width: {min: 0, max: 1},
                weight: {min: 0, max: 2},
                color: {min: 0, max: 1}
            },
            relative: {},
            width: {min: 1, max: 10},
            weight: {min: 1, max: 10}
        };
        if (type === "select")
            config[type].color = {min: "yellow", max: "green"};
        else if (type === "differ") {
            config[type].color_neg = {min: "yellow", max: "red"};
            config[type].color_pos = {min: "yellow", max: "green"};
        }
        else if (type === "compare")
            config[type].color = {min: "yellow", max: "red"};

        config[type].relative = {
            width: {
                min: this.getBound(graph.edges, "Frequency", "min", false, false),
                max: this.getBound(graph.edges, "Frequency", "max", false, false)},
            weight: {
                min: this.getBound(graph.edges, "Frequency", "min", false, true),
                max: this.getBound(graph.edges, "Frequency", "max", false, true)},
            color: {
                min: this.getBound(graph.edges, "Pearson", "min", type === "differ", false),
                max: this.getBound(graph.edges, "Pearson", "max", type === "differ", false)
            }
        };
    },
    deactivateUnused: function ( analysis) {
        if (!tremppi.data.setup.comparative) {
            $('#' + analysis + '_differ').html("");
            $('#' + analysis + '_compare').html("");
        }
        ;
    }
};

