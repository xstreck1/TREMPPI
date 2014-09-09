tremppi_viewer.getByID = function(type, id) {
    var list = elements[type];
    for (var i = 0; i < list.length; i++) {
        if (list[i].data.id === id)
            return list[i];
    }
};

tremppi_viewer.setHelp = function(text) {
    $("#graph_panel").html("");
    $("#graph_panel").append('<div id="graph_help" class="help">' + text + '</div>');
}

tremppi_viewer.setButtonControl = function() {
    $("#graph_panel").html("");
    $("#graph_panel").append('<div id="graph_control" class="controls"></div>');
    $("#graph_control").append('<button id="create">Create</button>');
    $("#create").click(function(event) {
        tremppi_viewer.activity_type = "create";
        tremppi_viewer.setHelp("Click on empty space to create a COMPONENT. Click on a component to put a source of a REGULATION.");
    });
    $("#graph_control").append('<button id="delete">Delete</button>');
    $("#delete").click(function(event) {
        tremppi_viewer.activity_type = "delete";
        tremppi_viewer.setHelp("Click on a COMPONENT or a REGULATION to delele it.");
    });
}

tremppi_viewer.elementChanged = function(row_id, column_id, old_val, new_val, row) {
    var element = tremppi_viewer.graph.$("#" + tremppi_viewer.current_selection.id);
    var val_name = tremppi_viewer.metadata[tremppi_viewer.current_selection.type][column_id].name;
    element.data(val_name, new_val);
};

tremppi_viewer.setSelectionScheme = function() {
    $("#graph_panel").html("");
    var element = tremppi_viewer.graph.$("#" + tremppi_viewer.current_selection.id);
    var data = [{id: "0", values: element.data()}];
    var selection = new EditableGrid("selection_" + tremppi_viewer.current_selection.id);
    selection.load({"metadata": tremppi_viewer.metadata[tremppi_viewer.current_selection.type], "data": data});
    selection.renderGrid("graph_panel", "testgrid");
    selection.modelChanged = tremppi_viewer.elementChanged;
};

tremppi_viewer.tapFunction = function(event) {
    if (event.cy == event.cyTarget) {
        // Create new component
        if (tremppi_viewer.activity_type === "create") {
            var new_node = {
                group: "nodes",
                data: {
                    Name: "New",
                    MaxActivity: 1
                },
                position: event.cyPosition
            };
            var new_node = tremppi_viewer.graph.add(new_node);
            tremppi_viewer.current_selection.type = "node";
            tremppi_viewer.current_selection.id = new_node.id();
            tremppi_viewer.setButtonControl();
        }
        // Deselect
        else {
            tremppi_viewer.current_selection.type = "";
            tremppi_viewer.current_selection.id = "";
            tremppi_viewer.setButtonControl();
        }
        tremppi_viewer.activity_type = "selection";
    } else if (event.cyTarget.isEdge()) {
        // delete edge
        if (tremppi_viewer.activity_type === "delete") {
            tremppi_viewer.graph.remove(tremppi_viewer.graph.$("#" + event.cyTarget.id()));
            tremppi_viewer.setButtonControl();
            tremppi_viewer.current_selection.type = "";
            tremppi_viewer.current_selection.id = "";
        }
        // Select edge
        else if (tremppi_viewer.activity_type === "selection") {
            tremppi_viewer.current_selection.type = "edge";
            tremppi_viewer.current_selection.id = event.cyTarget.id();
            tremppi_viewer.setSelectionScheme();
        }
    } else if (event.cyTarget.isNode()) {
        var last_node = tremppi_viewer.current_selection.id;
        tremppi_viewer.current_selection.type = "node";
        tremppi_viewer.current_selection.id = event.cyTarget.id();
        // Start an edge
        if (tremppi_viewer.activity_type === "create") {
            tremppi_viewer.activity_type = "end_regulation";
            tremppi_viewer.setHelp("Click on a component to put a target of a REGULATION.");
        }
        // Finish an edge
        else if (tremppi_viewer.activity_type === "end_regulation") {
            tremppi_viewer.graph.add({
                group: "edges",
                data: {
                    source: last_node,
                    target: tremppi_viewer.current_selection.id,
                    Threshold: 1,
                    Label: "tt"}
            });
            tremppi_viewer.activity_type = "selection";
            tremppi_viewer.setButtonControl();
        }
        // delete node
        if (tremppi_viewer.activity_type === "delete") {
            tremppi_viewer.graph.remove(tremppi_viewer.graph.$("#" + event.cyTarget.id()));
            tremppi_viewer.setButtonControl();
            tremppi_viewer.current_selection.type = "";
            tremppi_viewer.current_selection.id = "";
        }
        // select node
        else if (tremppi_viewer.activity_type === "selection") {
            tremppi_viewer.setSelectionScheme();
        }
    }
}