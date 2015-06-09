/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

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

tremppi.editor.setData = function (data) {
    tremppi.editor.setDefaultData(data);
    var glyphAll = function (elements, glyphFunction) {
        for (var i = 0; i < elements.length; i++) {
            glyphFunction(elements[i].data);
        }
    };
    glyphAll(data.nodes, tremppi.editor.glyphNode);
    glyphAll(data.edges, tremppi.editor.glyphEdge);
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
};

tremppi.editor.toolbarClass = function() {
    return {
        name: 'toolbar',
        items: [
            {type: 'button', id: 'save', caption: 'Save', img: 'icon-page', hint: 'Save the data'}
        ]
    };
};