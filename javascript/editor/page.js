/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.editor.widgetDOM = function () {
    $("#widget").append('<div id="graph_object"></div>');
};
tremppi.editor.controlsDOM = function () {
    w2ui.layout.set('main',
            {
                toolbar: {
                    name: 'toolbar',
                    items: [
                        {type: 'button', id: 'item1', caption: 'Item 1', img: 'icon-page', hint: 'Hint for item 1'},
                        {type: 'button', id: 'item2', caption: 'Item 2', icon: 'fa-wrench', hint: 'Hint for item 2'},
                        {type: 'button', id: 'item3', caption: 'Item 3', icon: 'fa-star-empty', hint: 'Hint for item 3'},
                        {type: 'button', id: 'item4', caption: 'Item 4', icon: 'fa-comments', hint: 'Hint for item 4'},
                        {type: 'button', id: 'item5', caption: 'Item 5', icon: 'fa-beaker', hint: 'Hint for item 5'}
                    ]
                }
            }
    );
    w2ui.layout.showToolbar('main');
};
tremppi.editor.controlsJS = function () {
    tremppi.editor.activity_type = "selection";
    tremppi.editor.current_selection = {type: "graph", name: "regulatory", id: ""};
    // tremppi.editor.setControls();
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
