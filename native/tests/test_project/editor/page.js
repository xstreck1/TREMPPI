/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.editor.setPage = function () {
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
        selectionType: "single"
    });

    tremppi.editor.setStyle(tremppi.editor.graph);
    tremppi.editor.graph.on('tap', tremppi.editor.selection);
    // Save after drag

    tremppi.editor.toolbar.onClick = tremppi.editor.toolbarClick;
    tremppi.editor.setBasic();
    // tremppi.editor.setControls();
};

tremppi.editor.setData = function (data) {
    var glyphAll = function (elements, glyphFunction) {
        for (var i = 0; i < elements.length; i++) {
            glyphFunction(elements[i].data);
        }
    };
    glyphAll(data.nodes, tremppi.editor.glyphNode);
    glyphAll(data.edges, tremppi.editor.glyphEdge);
    tremppi.editor.graph.load(data);
    tremppi.editor.graph.elements().unselect();
    // Add the save function
    tremppi.editor.graph.on('free', tremppi.editor.save);
};

tremppi.editor.save = function () {
    tremppi.data.configured = true;
    tremppi.data = tremppi.editor.graph.json().elements;
    tremppi.save();
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


tremppi.editor.defaultToolbar = function () {
    return {
        items: []
    };
};