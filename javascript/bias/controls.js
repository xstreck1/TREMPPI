/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi */

tremppi.bias.toolbarClick = function (event) {
    if (event.target === 'all') {
        tremppi.report.showAll();
        tremppi.setItem('panel', 'all');
        for (var i = 0; i < tremppi.report.panels.length; i++) {
            tremppi.bias.loadLabels(tremppi.report.panels[i]);
        }
    } else if (event.target === 'left' | event.target === 'mid' | event.target === 'right') {
        tremppi.setItem('panel', event.target);
        tremppi.report.showPanel(event.target);
        tremppi.bias.loadLabels(event.target);
    } else if (event.target.slice(0, 7) === 'select:') {
        var selected = event.target.slice(7);
        tremppi.setItem('selected', selected);
        tremppi.report.pickData(selected, 'left');
    } else if (event.target.slice(0, 8) === 'compare:') {
        var compared = event.target.slice(8);
        tremppi.setItem('compared', compared);
        tremppi.report.pickData(compared, 'right');
    } else if (event.target === 'relative') {
        var checked = !event.item.checked;
        tremppi.setItem('relative', checked);
        for (var i = 0; i < tremppi.report.panels.length; i++) {
            tremppi.bias.applyVisuals(tremppi.report.panels[i]);
            tremppi.bias.loadLabels(tremppi.report.panels[i]);
        }
    }
};

tremppi.bias.Values = {
    caption_c: "a) statistical properties of the selected parametrizations set, c) properties of the compared parametrizations set, b) difference between the two. "
            + "F stands for frequency of occurrence of an edge in the given set, P for the Pearson correlation coefficient between the value of the regulator and the parameter value. "
            + "Blunt edges denote positive and arrows describe the negative correlation value in the graphs a) and c). "
            + "In the graph b) the positive change in frequency is depicted by full edge, the negative by the dashed one. "
            + "The dotted edge is used when the difference in frequency is zero.",
    caption_nonc: "a) statistical properties of the selected parametrizations set. "
            + "F stands for frequency of occurrence of an edge in the given set, P for the Pearson correlation coefficient between the value of the regulator and the parameter value. "
            + "Blunt edges denote positive and arrows describe the negative correlation value. "
};

tremppi.bias.getConfiguraion = function () {
    return {
        left: {
            absolute: {
                node_pos: {min: 0, max: 1},
                node_neg: {min: -1, max: 0},
                edge_pos: {min: 0, max: 1},
                edge_neg: {min: -1, max: 0}
            },
            relative: {
                node_pos: {min: 0, max: 1},
                node_neg: {min: -1, max: 0},
                edge_pos: {min: 0, max: 1},
                edge_neg: {min: -1, max: 0}
            },
            node_pos: {min: tremppi.bias.node_null_color, max: "green"},
            node_neg: {min: "red", max: tremppi.bias.node_null_color},
            edge_pos: {min: tremppi.bias.edge_null_color, max: "green"},
            edge_neg: {min: "red", max: tremppi.bias.edge_null_color}
        },
        mid: {
            absolute: {
                node_pos: {min: 0, max: 2},
                node_neg: {min: -2, max: 0},
                edge_pos: {min: 0, max: 2},
                edge_neg: {min: -2, max: 0}
            },
            relative: {
                node_pos: {min: 0, max: 2},
                node_neg: {min: -2, max: 0},
                edge_pos: {min: 0, max: 2},
                edge_neg: {min: -2, max: 0}
            },
            node_pos: {min: tremppi.bias.node_null_color, max: "green"},
            node_neg: {min: "red", max: tremppi.bias.node_null_color},
            edge_pos: {min: tremppi.bias.edge_null_color, max: "green"},
            edge_neg: {min: "red", max: tremppi.bias.edge_null_color}
        },
        right: {
            absolute: {
                node_pos: {min: 0, max: 1},
                node_neg: {min: -1, max: 0},
                edge_pos: {min: 0, max: 1},
                edge_neg: {min: -1, max: 0}
            },
            relative: {
                node_pos: {min: 0, max: 1},
                node_neg: {min: -1, max: 0},
                edge_pos: {min: 0, max: 1},
                edge_neg: {min: -1, max: 0}
            },
            node_pos: {min: tremppi.bias.node_null_color, max: "green"},
            node_neg: {min: "red", max: tremppi.bias.node_null_color},
            edge_pos: {min: tremppi.bias.edge_null_color, max: "green"},
            edge_neg: {min: "red", max: tremppi.bias.edge_null_color}
        }
    };
};


tremppi.bias.getBound = function (edges, param, fun) {
    var result = 0;

    for (var edge_no = 0; edge_no < edges.length; edge_no++) {
        var value = edges[edge_no].data[param];
        result = fun(Math.abs(result), Math.abs(value));
    }

    return result;
};

// gives the ranges of values and their respective mapping
tremppi.bias.configure = function (type, elements) {
    var edges = elements.edges;
    var nodes = elements.nodes;
    tremppi.bias.config[type].relative = {
        node_pos: {
            min: 0,
            max: tremppi.bias.getBound(nodes, "Bias", Math.max)
        },
        node_neg: {
            min: tremppi.bias.getBound(nodes, "Bias", Math.max) * -1,
            max: 0
        },
        edge_pos: {
            min: 0,
            max: tremppi.bias.getBound(edges, "Pearson", Math.max)
        },
        edge_neg: {
            min: tremppi.bias.getBound(edges, "Pearson", Math.max) * -1,
            max: 0
        }
    };
};