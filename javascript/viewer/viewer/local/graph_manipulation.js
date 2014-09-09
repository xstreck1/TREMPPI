tremppi_viewer.positionNodes = function(graph) {
    for (var i = 0; i < elements.nodes.length; i++) {
        var element = graph.$("#" + elements.nodes[i].data.id);
        var position = elements.nodes[i].position;
        element.position(position);
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
    $("#new_regulation").click(function(event) {
        tremppi_viewer.activity_type = "new_regulation";
    });
}

tremppi_viewer.elementChanged = function(row_id, column_id, old_val, new_val, row) {
    var element = tremppi_viewer.graph.$("#" + tremppi_viewer.current_selection.name);
    var val_name = tremppi_viewer.metadata[tremppi_viewer.current_selection.type][column_id].name;
    element.data(val_name, new_val);
};

tremppi_viewer.setSelectionScheme = function() {
    $("#graph_panel").html("");
    var element = tremppi_viewer.graph.$("#" + tremppi_viewer.current_selection.name);
    var data = [{id: "0", values: element.data()}];
    var selection = new EditableGrid("selection_" + tremppi_viewer.current_selection.name);
    selection.load({"metadata": tremppi_viewer.metadata[tremppi_viewer.current_selection.type], "data": data});
    selection.renderGrid("graph_panel", "testgrid");
    selection.modelChanged = tremppi_viewer.elementChanged;
    $("#graph_panel").append('<br />');
};

tremppi_viewer.tapFunction = function(event) {
    if (event.cy == event.cyTarget) {
        if (tremppi_viewer.activity_type === "new_component") {
            tremppi_viewer.graph.add({
                group: "nodes",
                data: {Name: "New"},
                position: event.cyPosition
            });
        } else {
            tremppi_viewer.setButtonControl();
        }
        tremppi_viewer.activity_type === "selection";
    } else if (event.cyTarget.isEdge()) {
        tremppi_viewer.current_selection.type = "edge";
        tremppi_viewer.current_selection.name = event.cyTarget.id();
        tremppi_viewer.setSelectionScheme();
    } else if (event.cyTarget.isNode()) {
        tremppi_viewer.current_selection.type = "node";
        tremppi_viewer.current_selection.name = event.cyTarget.id();
        tremppi_viewer.setSelectionScheme();
    }
}