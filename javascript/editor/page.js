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
    var dataSet = false;
    if (typeof data.nodes === 'undefined') {
        data.nodes = [];
        dataSet= true;
    }
    if (typeof data.edges === 'undefined') {
        data.edges = [];
        dataSet = true;
    }
    if (typeof data.use_normalized === 'undefined') {
        data.use_normalized = true;
        dataSet = true;
    }
    
    if (dataSet) {
        tremppi.save(); // Save with the default data if those were missing
    }
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


tremppi.editor.compareData = function (A, B) {
    if (A.use_normalized !== B.use_normalized ||
        A.nodes.length !== B.nodes.length || 
        A.edges.length !== B.edges.length)
        return false;
        
    for (var node_i = 0; node_i < A.nodes.length; node_i++) {
        var nodeA = A.nodes[node_i];
        var nodeB = B.nodes[node_i];
        if (nodeA.position.x !== nodeB.position.x ||
            nodeA.position.y !== nodeB.position.y ||   
            nodeA.data.id !== nodeB.data.id ||
            nodeA.data.Label !== nodeB.data.Label || 
            nodeA.data.Name !== nodeB.data.Name ||
            nodeA.data.MaxActivity !== nodeB.data.MaxActivity) 
        {
            return false;
        }
    }
    
    for (var edge_i = 0; edge_i < A.edges.length; edge_i++) {
        var edgeA = A.edges[edge_i];
        var edgeB = B.edges[edge_i];
        if (edgeA.data.id !== edgeA.data.id ||
            edgeA.data.Label !== edgeA.data.Label ||
            edgeA.data.target !== edgeA.data.target ||
            edgeA.data.source !== edgeA.data.source ||
            edgeA.data.Threshold !== edgeA.data.Threshold) 
        {
            return false;
        }
    }
    
    return true;
};