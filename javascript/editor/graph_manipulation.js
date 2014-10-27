tremppi_editor.getByID = function(type, id) {
    var list = elements[type];
    for (var i = 0; i < list.length; i++) {
        if (list[i].data.id === id)
            return list[i];
    }
};

tremppi_editor.setHelp = function(text) {
    $("#graph_panel").html("");
    $("#graph_panel").append('<div id="graph_help" class="help">' + text + '</div>');
};

tremppi_editor.setButtonControl = function() {
    $("#graph_panel").html("");
    $("#graph_panel").append('<div id="graph_control" class="controls"></div>');
    $("#graph_control").append('<button id="create">Create</button>');
    $("#create").click(function(event) {
        tremppi_editor.activity_type = "create";
        tremppi_editor.setHelp("Click on empty space to create a COMPONENT. Click on a component to put a source of a REGULATION.");
    });
    $("#graph_control").append('<button id="delete">Delete</button>');
    $("#delete").click(function(event) {
        tremppi_editor.activity_type = "delete";
        tremppi_editor.setHelp("Click on a COMPONENT or a REGULATION to delele it.");
    });
};

tremppi_editor.elementChanged = function(row_id, column_id, old_val, new_val, row) {
    var element = tremppi_editor.graph.$("#" + tremppi_editor.current_selection.id);
    var val_name = tremppi_editor.metadata[tremppi_editor.current_selection.type][column_id].name;
    element.data(val_name, new_val);
    tremppi_editor.saveGraph();
};

tremppi_editor.saveGraph = function() {
    elements = tremppi_editor.graph.json().elements;
    tremppi_common.save("elements");
}

tremppi_editor.setSelectionScheme = function() {
    $("#graph_panel").html("");
    var element = tremppi_editor.graph.$("#" + tremppi_editor.current_selection.id);
    var data = [{id: "0", values: element.data()}];
    var selection = new EditableGrid("selection_" + tremppi_editor.current_selection.id);
    selection.load({"metadata": tremppi_editor.metadata[tremppi_editor.current_selection.type], "data": data});
    selection.renderGrid("graph_panel", "testgrid");
    selection.modelChanged = tremppi_editor.elementChanged;
};

tremppi_editor.tapFunction = function(event) {
    // Click outside of all components
    if (event.cy == event.cyTarget) {
        // Create new component
        if (tremppi_editor.activity_type === "create") {
            var new_node = {
                group: "nodes",
                data: {
                    Name: "New",
                    MaxActivity: 1
                },
                position: event.cyPosition
            };
            var new_node = tremppi_editor.graph.add(new_node);
            tremppi_editor.saveGraph();
            tremppi_editor.current_selection.type = "node";
            tremppi_editor.current_selection.id = new_node.id();
            tremppi_editor.setButtonControl();
        }
        // Deselect
        else {
            tremppi_editor.current_selection.type = "";
            tremppi_editor.current_selection.id = "";
            tremppi_editor.setButtonControl();
        }
        tremppi_editor.activity_type = "selection";
    } else if (event.cyTarget.isEdge()) {
        // delete edge
        if (tremppi_editor.activity_type === "delete") {
            tremppi_editor.graph.remove(tremppi_editor.graph.$("#" + event.cyTarget.id()));
            tremppi_editor.saveGraph();
            tremppi_editor.setButtonControl();
            tremppi_editor.current_selection.type = "";
            tremppi_editor.current_selection.id = "";
        }
        // Select edge
        else if (tremppi_editor.activity_type === "selection") {
            tremppi_editor.current_selection.type = "edge";
            tremppi_editor.current_selection.id = event.cyTarget.id();
            tremppi_editor.setSelectionScheme();
        }
    } else if (event.cyTarget.isNode()) {
        var last_node = tremppi_editor.current_selection.id;
        tremppi_editor.current_selection.type = "node";
        tremppi_editor.current_selection.id = event.cyTarget.id();
        // Start an edge
        if (tremppi_editor.activity_type === "create") {
            tremppi_editor.activity_type = "end_regulation";
            tremppi_editor.setHelp("Click on a component to put a target of a REGULATION.");
        }
        // Finish an edge
        else if (tremppi_editor.activity_type === "end_regulation") {
            tremppi_editor.graph.add({
                group: "edges",
                data: {
                    source: last_node,
                    target: tremppi_editor.current_selection.id,
                    Threshold: 1,
                    Label: "tt"}
            });
            tremppi_editor.saveGraph();
            tremppi_editor.activity_type = "selection";
            tremppi_editor.setButtonControl();
        }
        // delete node
        if (tremppi_editor.activity_type === "delete") {
            tremppi_editor.graph.remove(tremppi_editor.graph.$("#" + event.cyTarget.id()));
            tremppi_editor.saveGraph();
            tremppi_editor.setButtonControl();
            tremppi_editor.current_selection.type = "";
            tremppi_editor.current_selection.id = "";
        }
        // select node
        else if (tremppi_editor.activity_type === "selection") {
            tremppi_editor.setSelectionScheme();
        }
    }
};