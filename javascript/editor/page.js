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
    tremppi.editor.addValues(tremppi.editor.graph);
    tremppi.editor.setStyle(tremppi.editor.graph);
    tremppi.editor.graph.on('tap', tremppi.editor.selection);
    // Save after drag

    tremppi.editor.setBasic();
    tremppi.editor.activity_type = "selection";
    tremppi.editor.current_selection = {type: "graph", name: "regulatory", id: ""};
    // tremppi.editor.setControls();
};

tremppi.editor.setData = function () {
    tremppi.editor.graph.load(tremppi.data);
    // Add the save function
    tremppi.editor.graph.on('free', tremppi.editor.save);
};

tremppi.editor.save = function () {
    tremppi.data = tremppi.editor.graph.json().elements;
    tremppi.save();
};

tremppi.editor.layout = function () {
    $("#graph_object").cytoscape('get').resize();
};

tremppi.editor.defaultData = {
    nodes: [],
    edges: []
};


tremppi.editor.defaultToolbar = {
    items: [],
    onClick: tremppi.editor.toolbarClick
};