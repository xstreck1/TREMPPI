/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi */

tremppi.regulations.toolbarClick = function (event) {
    if (event.target === 'all') {
        tremppi.report.showAll();
        tremppi.setItem('panel', 'all');
        for (var i = 0; i < tremppi.report.panels.length; i++) {
            tremppi.regulations.loadLabels(tremppi.report.panels[i]);
        }
    } else if (event.target === 'left' | event.target === 'mid' | event.target === 'right') {
        tremppi.setItem('panel', event.target);
        tremppi.report.showPanel(event.target);
        tremppi.regulations.loadLabels(event.target);
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
            tremppi.regulations.applyVisuals(tremppi.report.panels[i]);
            tremppi.regulations.loadLabels(tremppi.report.panels[i]);
        }
    } else if (event.target === 'weighted') {
        var checked = !event.item.checked;
        tremppi.setItem('weighted', checked);
        for (var i = 0; i < tremppi.report.panels.length; i++) {
            tremppi.regulations.applyVisuals(tremppi.report.panels[i]);
            tremppi.regulations.loadLabels(tremppi.report.panels[i]);
        }
    }
};

tremppi.regulations.Values = {
    caption_c: "a) statistical properties of the selected parametrizations set, c) properties of the compared parametrizations set, b) difference between the two. "
            + "F stands for frequency of occurrence of an edge in the given set, P for the Pearson correlation coefficient between the value of the regulator and the parameter value. "
            + "Blunt edges denote positive and arrows describe the negative correlation value in the graphs a) and c). "
            + "In the graph b) the positive change in frequency is depicted by full edge, the negative by the dashed one. "
            + "The dotted edge is used when the difference in frequency is zero.",
    caption_nonc: "a) statistical properties of the selected parametrizations set. "
            + "F stands for frequency of occurrence of an edge in the given set, P for the Pearson correlation coefficient between the value of the regulator and the parameter value. "
            + "Blunt edges denote positive and arrows describe the negative correlation value. "
};

tremppi.regulations.getConfiguraion = function () {
    return {
        left: {
            absolute: {
                width: {min: 0, max: 1},
                weight: {min: 0, max: 2},
                color_pos: {min: 0, max: 1},
                color_neg: {min: -1, max: 0}
            },
            relative: {
                width: {min: 0, max: 1},
                weight: {min: 0, max: 2},
                color_pos: {min: 0, max: 1},
                color_neg: {min: -1, max: 0}
            },
            width: {min: 1, max: 10},
            weight: {min: 1, max: 10},
            color_neg: {min: "red", max: "yellow"},
            color_pos: {min: "yellow", max: "green"}
        },
        mid: {
            absolute: {
                width: {min: 0, max: 1},
                weight: {min: 0, max: 2},
                color_pos: {min: 0, max: 2},
                color_neg: {min: -2, max: 0}
            },
            relative: {
                width: {min: 0, max: 1},
                weight: {min: 0, max: 2},
                color_pos: {min: 0, max: 2},
                color_neg: {min: -2, max: 0}
            },
            width: {min: 1, max: 10},
            weight: {min: 1, max: 10},
            color_neg: {min: "red", max: "yellow"},
            color_pos: {min: "yellow", max: "green"}
        },
        right: {
            absolute: {
                width: {min: 0, max: 1},
                weight: {min: 0, max: 2},
                color_pos: {min: 0, max: 1},
                color_neg: {min: -1, max: 0}
            },
            relative: {
                width: {min: 0, max: 1},
                weight: {min: 0, max: 2},
                color_pos: {min: 0, max: 1},
                color_neg: {min: -1, max: 0}
            },
            width: {min: 1, max: 10},
            weight: {min: 1, max: 10},
            color_neg: {min: "red", max: "yellow"},
            color_pos: {min: "yellow", max: "green"}
        }
    };
};


tremppi.regulations.getBound = function (edges, param, fun, weighted) {
    var result = 0;

    for (var edge_no = 0; edge_no < edges.length; edge_no++) {
        var value = edges[edge_no].data[param];
        if (typeof weighted !== 'undefined' && weighted) {
            value /= edges[edge_no].data["ExpectedFreq"];
        }
        result = fun(Math.abs(result), Math.abs(value));
    }

    return result;
};

// gives the ranges of values and their respective mapping
tremppi.regulations.configure = function (type, elements) {
    var edges = elements.edges;
    tremppi.regulations.config[type].relative = {
        width: {
            min: tremppi.regulations.getBound(edges, "Frequency", Math.min),
            max: tremppi.regulations.getBound(edges, "Frequency", Math.max)},
        weight: {
            min: tremppi.regulations.getBound(edges, "Frequency", Math.min, true),
            max: tremppi.regulations.getBound(edges, "Frequency", Math.max, true)},
        color_pos: {
            min: 0,
            max: tremppi.regulations.getBound(edges, "Pearson", Math.max)
        },
        color_neg: {
            min: tremppi.regulations.getBound(edges, "Pearson", Math.max) * -1,
            max: 0
        }
    };
};