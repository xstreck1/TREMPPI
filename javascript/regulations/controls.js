/******************************************************************************
Created by Adam Streck, 2013-2015, adam.streck@fu-berlin.de

This file is part of the Toolkit for Reverse Engineering of Molecular Pathways
via Parameter Identification (TREMPPI)

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

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
