tremppi_viewer.positionNodes = function(graph) {
    for (var i = 0; i < elements.nodes.length; i++) {
        var pos = {};
        if (elements.nodes[i].x)
            pos.x = elements.nodes[i].x;
        if (elements.nodes[i].y)
            pos.y = elements.nodes[i].y;
        var filter = 'node[id = "' + elements.nodes[i].data.id + '"]';
        var selected = graph.filter(filter);
        selected.position(pos);
    }
};

tremppi_viewer.getByID = function(type, id) {
    var list = elements[type];
    for (var i = 0; i < list.length; i++) {
        if (list[i].data.id === id)
            return list[i];
    }
};

tremppi_viewer.setButtonControl = function() {
    $("#graph_panel").html("");
    $("#graph_panel").append('<div id="graph_control" class="controls"></div>');
    $("#graph_control").append('<button id="new_component">New Component</button>');
    $("#new_component").click(function(event) {
        tremppi_viewer.activity_type = "new_component";
    });
    $("#graph_control").append('<button id="new_regulation">New Regulation</button>');
    $("#new_component").click(function(event) {
        tremppi_viewer.activity_type = "new_regulation";
    });
}

tremppi_viewer.setSelectionScheme = function(graph) {
    $("#graph_panel").html("");
    var element = graph.$("#" + tremppi_viewer.current_selection.name);
    var data = [{id: "0", values: element.data()}];
    var selection = new EditableGrid("selection_" + tremppi_viewer.current_selection.name);
    selection.load({"metadata": tremppi_viewer.metadata[tremppi_viewer.current_selection.type], "data": data});
    selection.renderGrid("graph_panel", "testgrid");
    $("#graph_panel").append('<br />');
    // selection.modelChanged = tremppi_viewer.modelChanged;
};

tremppi_viewer.tapFunction = function(event) {
    if (event.cy == event.cyTarget) {
        tremppi_viewer.setButtonControl();
    } else if (event.cyTarget.isEdge()) {
        tremppi_viewer.current_selection.type = "edge";
        tremppi_viewer.current_selection.name = event.cyTarget.id();
        tremppi_viewer.setSelectionScheme(event.cy);
    } else if (event.cyTarget.isNode()) {
        tremppi_viewer.current_selection.type = "node";
        tremppi_viewer.current_selection.name = event.cyTarget.id();
        tremppi_viewer.setSelectionScheme(event.cy);
    }
}