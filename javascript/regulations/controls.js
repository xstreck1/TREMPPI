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

//tremppi.regulations.Values = {
//    caption_c: "a) statistical properties of the selected parametrizations set, c) properties of the compared parametrizations set, b) difference between the two. "
//            + "F stands for frequency of occurrence of an edge in the given set, P for the Pearson correlation coefficient between the value of the regulator and the parameter value. "
//            + "Blunt edges denote positive and arrows describe the negative correlation value in the graphs a) and c). "
//            + "In the graph b) the positive change in frequency is depicted by full edge, the negative by the dashed one. "
//            + "The dotted edge is used when the difference in frequency is zero.",
//    caption_nonc: "a) statistical properties of the selected parametrizations set. "
//            + "F stands for frequency of occurrence of an edge in the given set, P for the Pearson correlation coefficient between the value of the regulator and the parameter value. "
//            + "Blunt edges denote positive and arrows describe the negative correlation value. "
//};
