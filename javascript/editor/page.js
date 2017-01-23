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

tremppi.editor.page = function () {
    $("#widget").append('<div id="graph_object"></div>');

    tremppi.editor.graph = cytoscape({
        container: $('#graph_object')[0],
        layout: {name: 'preset', positions: function (node) {
                var pos = node.position();
                if (typeof pos === "undefined" || typeof pos.x === "undefined" || typeof pos.y === "undefined")
                    return {x: 0, y: 0};
                else
                    return pos;
            }
        },
        selectionType: "single",
        wheelSensitivity: 0.2
    });

    tremppi.editor.setStyle(tremppi.editor.graph);
};

tremppi.editor.getData = function () {
    var data = tremppi.editor.graph.json().elements;
    data.use_normalized = tremppi.editor.use_normalized;
    return data;
};

tremppi.editor.setData = function (data) {
    tremppi.editor.setDefaultData(data);
    var glyphAll = function (elements, glyphFunction) {
        for (var i = 0; i < elements.length; i++) {
            glyphFunction(elements[i].data);
        }
    };
    glyphAll(data.nodes, tremppi.editor.glyphNode);
    glyphAll(data.edges, tremppi.editor.glyphEdge);
    tremppi.editor.use_normalized = data.use_normalized;
    tremppi.editor.graph.load(data);
    tremppi.editor.graph.elements().unselect();

    // Enable controls
    tremppi.editor.controls();
};

tremppi.editor.layout = function () {
    $("#graph_object").cytoscape('get').resize();
};

tremppi.editor.setDefaultData = function (data) {
    if (typeof data.nodes === 'undefined')
        data.nodes = [];
    if (typeof data.edges === 'undefined')
        data.edges = [];
    if (typeof data.use_normalized === 'undefined') 
        data.use_normalized = true;
};

tremppi.editor.toolbarClass = function () {
    return {
        name: 'toolbar',
        items: tremppi.editor.isEnumerated() ? 
        [
            {type: 'break', id: 'break1'}
        ] : [
            
            {type: 'button', id: 'create', icon: 'w2ui-icon-plus', caption: 'Add', hint: 'click on an empty space to create a node, click on a node to start an edge'},
            {type: 'button', id: 'delete', icon: 'w2ui-icon-cross', caption: 'Delete', hint: 'delete an element'}
        ]
    };
};
tremppi.editor.beforeUnload = function() {
    tremppi.save();
};