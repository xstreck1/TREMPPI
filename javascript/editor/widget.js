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

tremppi.editor.lables = [
    "Activating",
    "Activating Only",
    "Not Inhibiting",
    "Inhibiting",
    "Inhibiting Only",
    "Not Activating",
    "Observable",
    "Not Observable",
    "Monotone",
    "Not Monotone",
    "Monotone Observable",
    "Free"
            // "ff" not added
            // "(-&+)|(!+&!-)" not added
            // "+|!-" 
            // "-|!+" not added
];

tremppi.editor.setStyle = function (graph) {
    var selected_col = "blue";

    graph.style().selector('node').css({
        'content': 'data(Label)',
        'text-valign': 'center',
        'color': 'black',
        'background-color': 'white',
        'border-color': 'black',
        'border-width': 1,
        'shape': 'rectangle',
        'width': 100,
        'height': 25,
        'font-size': 15
    }).selector('edge').css({
        'content': 'data(Threshold)',
        'text-outline-color': 'black',
        'text-outline-width': 1,
        'color': 'white',
        'line-color': 'data(line_color)',
        'line-style': 'data(line_style)',
        'target-arrow-shape': 'data(target_arrow_shape)',
        'target-arrow-color': 'data(line_color)',
        'width': 2
    }).selector(':selected').css({
        'color': selected_col
    }).update();
};

tremppi.editor.glyphEdge = function (edge) {
    var label = edge.Label;
    if (label === "Activating" || label === "Inhibiting" || label === "Activating Only" || label === "Inhibiting Only"
            || label === "Observable" || label === "Not Monotone" || label === "Monotone Observable")
        edge.line_style = "solid";
    else
        edge.line_style = "dashed";
    if (label === "Not Inhibiting" || label === "Activating Only")
        edge.line_color = "green";
    else if (label === "Not Activating" || label === "Inhibiting Only")
        edge.line_color = "red";
    else
        edge.line_color = "black";
    if (label === "Activating" || label === "Activating Only")
        edge.target_arrow_shape = "triangle";
    else if (label === "Inhibiting" || label === "Inhibiting Only")
        edge.target_arrow_shape = "tee";
    else
        edge.target_arrow_shape = "circle";
};

tremppi.editor.glyphNode = function (node) {
    node.Label = node.Name + ":" + node.MaxActivity;
};

tremppi.editor.newNode = function (click_position) {
    return {
        group: "nodes",
        data: {
            Name: "New",
            MaxActivity: 1
        },
        position: click_position
    };
};

tremppi.editor.newEdge = function (source, target) {
    return {
        group: "edges",
        data: {
            source: source.id(),
            target: target.id(),
            Threshold: 1,
            Label: "Free"
        }
    };
};
