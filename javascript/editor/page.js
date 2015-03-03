/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.editor.widgetDOM = function () {
    $("#widget").append('<div id="graph_object"></div>');
};
tremppi.editor.controlsDOM = function () {
};
tremppi.editor.controlsJS = function () {
    tremppi.editor.activity_type = "selection";
    tremppi.editor.current_selection = {type: "graph", name: "regulatory", id: ""};
    tremppi.editor.setControls();
};
tremppi.editor.widgetJS = function () { // on dom ready
    tremppi.editor.graph = cytoscape({
        container: $('#graph_object')[0],
        layout: {name: tremppi.cytoscape.hasAllPositions(tremppi.data.nodes) ? 'preset' : 'grid'},
        elements: tremppi.data,
        selectionType: "single"
    });
    tremppi.editor.addValues(tremppi.editor.graph);
    tremppi.editor.setStyle(tremppi.editor.graph);
    tremppi.editor.graph.on('tap', tremppi.editor.tapFunction);
    // Save after drag
    tremppi.editor.graph.on('free', function () {
        tremppi.data = tremppi.editor.graph.json().elements;
        tremppi.save();
    });
};

tremppi.editor.layout = function () {
    $("#graph_object").cytoscape('get').resize();
};

tremppi.editor.defaultData = {
    nodes: [],
    edges: []
};
