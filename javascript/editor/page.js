/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.editor.widgetDOM = function () {
    $("#widget").append('<div id="graph_object"></div>');
};
tremppi.editor.controlsDOM = function () {
    $("#controls").append('<div id = "component_manipulation"></div>');
    $("#controls").append('<div id = "graph_selection"></div>');
    $("#controls").height(130);
};
tremppi.editor.controlsJS = function () {
    tremppi.editor.activity_type = "selection";
    tremppi.editor.current_selection = {type: "graph", name: "regulatory", id: ""};
    tremppi.editor.setButtons();
    tremppi.editor.setSelectionScheme();
};
tremppi.editor.widgetJS = function () { // on dom ready
    var selected_col = '#0000AA';
    tremppi.editor.graph = cytoscape({
        container: $('#graph_object')[0],
        layout: {name: tremppi.cytoscape.hasAllPositions(tremppi.data.nodes) ? 'preset' : 'grid'},
        elements: tremppi.data,
        selectionType: "single"
    });
    tremppi.editor.changeVisuals(tremppi.editor.graph);
    tremppi.editor.graph.on('tap', tremppi.editor.tapFunction);
    tremppi.editor.graph.style().selector('node').css({
        'content': 'data(Label)',
        'text-valign': 'center',
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
        'border-color': selected_col,
        'line-color': selected_col,
        'target-arrow-color': selected_col
    }).update();
};

tremppi.editor.layout = function () {
    $("#graph_object").cytoscape('get').resize();
};
tremppi.editor.default = {
    nodes: [],
    edges: []
};
