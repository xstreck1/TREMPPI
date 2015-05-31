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
                color: {min: 0, max: 1}
            },
            relative: {
                width: {min: 0, max: 1},
                weight: {min: 0, max: 2},
                color: {min: 0, max: 1}
            },
            width: {min: 1, max: 10},
            weight: {min: 1, max: 10},
            color: {min: "yellow", max: "green"}
        },
        mid: {
            absolute: {
                width: {min: 0, max: 1},
                weight: {min: 0, max: 2},
                color: {min: 0, max: 1}
            },
            relative: {
                width: {min: 0, max: 1},
                weight: {min: 0, max: 2},
                color: {min: 0, max: 1}
            },
            width: {min: 1, max: 10},
            weight: {min: 1, max: 10},
            color_neg: {min: "yellow", max: "red"},
            color_pos: {min: "yellow", max: "green"}
        },
        right: {
            absolute: {
                width: {min: 0, max: 1},
                weight: {min: 0, max: 2},
                color: {min: 0, max: 1}
            },
            relative: {
                width: {min: 0, max: 1},
                weight: {min: 0, max: 2},
                color: {min: 0, max: 1}
            },
            width: {min: 1, max: 10},
            weight: {min: 1, max: 10},
            color: {min: "yellow", max: "red"}
        }
    };
};

tremppi.regulations.compareFunction = function (is_max, signed) {
    if (is_max) {
        if (signed) {
            return function (x, y) {
                return Math.max(Math.abs(x), Math.abs(y));
            };
        }
        else {
            return function (x, y) {
                return Math.max(x, y);
            };
        }
    }
    else {
        if (signed) {
            return function (x, y) {
                return 0;
            };
        }
        else {
            return function (x, y) {
                return Math.min(Math.abs(x), Math.abs(y));
            };

        }
    }
};

tremppi.regulations.getBound = function (edges, param, fun, signed, weighted) {
    var result = 0;

    var cmp_function = tremppi.regulations.compareFunction(fun === 'max', signed);

    for (var edge_no = 0; edge_no < edges.length; edge_no++) {
        var value = edges[edge_no].data(param);
        if (weighted)
            value /= edges[edge_no].data("ExpectedFreq");
        result = cmp_function(result, value);
    }

    return result;
};

// gives the ranges of values and their respective mapping
tremppi.regulations.configure = function (type) {
    var edges = tremppi.regulations[type].edges();
    tremppi.regulations.config[type].relative = {
        width: {
            min: tremppi.regulations.getBound(edges, "Frequency", "min", false, false),
            max: tremppi.regulations.getBound(edges, "Frequency", "max", false, false)},
        weight: {
            min: tremppi.regulations.getBound(edges, "Frequency", "min", false, true),
            max: tremppi.regulations.getBound(edges, "Frequency", "max", false, true)},
        color: {
            min: tremppi.regulations.getBound(edges, "Pearson", "min", type === "mid", false),
            max: tremppi.regulations.getBound(edges, "Pearson", "max", type === "mid", false)
        }
    };
};