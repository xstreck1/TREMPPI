tremppi_editor.getByID = function (type, id) {
    var list = elements[type];
    for (var i = 0; i < list.length; i++) {
        if (list[i].data.id === id)
            return list[i];
    }
};

tremppi_editor.setControl = function () {
    var setHelp = function (text) {
        $("#control_panel").append('<div id="graph_help" class="help">' + text + '</div>');
    };
    $("#control_panel").html("");
    if (tremppi_editor.activity_type === "selection") {
        $("#control_panel").append('<div id="graph_control" ></div>');
        $("#graph_control").append('<button id="create">Create</button> ELEMENT / ');
        $("#create").click(function (event) {
            tremppi_editor.activity_type = "create";
            tremppi_editor.setControl();
        });
        $("#graph_control").append('<button id="delete">Delete</button> ELEMENTS');
        $("#delete").click(function (event) {
            tremppi_editor.activity_type = "delete";
            tremppi_editor.setControl();
        });
    } else if (tremppi_editor.activity_type === "create") {
        setHelp("Click on empty space to create a COMPONENT. Click on a component to put a source of a REGULATION.");
    } else if (tremppi_editor.activity_type === "delete") {
        setHelp("Click on a COMPONENT or a REGULATION to delele it. Click on the plane to stop deleting.");
    } else if (tremppi_editor.activity_type === "end_regulation") {
        setHelp("Click on a component to put a target of a REGULATION.");
    }
    
    $("#control_pad").height(($("#control_panel").height() + 20) + "px");
};

tremppi_editor.elementChanged = function (row_id, column_id, old_val, new_val, row) {
    var element = tremppi_editor.graph.$("#" + tremppi_editor.current_selection.id);
    var val_name = tremppi_editor.metadata[tremppi_editor.current_selection.type][column_id].name;
    element.data(val_name, new_val);
    tremppi_editor.saveGraph();
    tremppi_editor.changeVisuals();
};

tremppi_editor.changeVisuals = function() {
    var edges = $('#graph_object').cytoscape('get').elements("edge");
    for (var i = 0; i < edges.length; i++) {
        var data = edges[i].data();
        var label = data.Label;
        if (label === "+" || label === "-" || label === "+&!-"  || label === "-&!+"
         || label === "-|+" || label === "-&+" || label === "(-&!+)|(+&!-)")
            data.line_style = "solid";
        else
            data.line_style = "dashed";
        if (label === "!-" || label === "+&!-")
            data.line_color = "green";
        else if (label === "!+" || label === "-&!+")
            data.line_color = "red";
        else
            data.line_color = "black";
        if (label === "+" || label === "+&!-")
            data.target_arrow_shape = "triangle";
        else if (label === "-" || label === "-&!+")
            data.target_arrow_shape = "tee";  
        else
            data.target_arrow_shape = "circle";
    }
    var nodes = $('#graph_object').cytoscape('get').elements("node");
    for (var i = 0; i < nodes.length; i++) {
        var node_data = nodes[i].data();
        node_data.Label = node_data.Name + ":" + node_data.MaxActivity;
    }
}

tremppi_editor.saveGraph = function () {
    editor = tremppi_editor.graph.json().elements;
    if (tremppi_editor.graph.elements("node").length === 0)
        editor.nodes = [];
    if (tremppi_editor.graph.elements("edge").length === 0)
        editor.edges = [];
    tremppi_common.save("editor");
};

tremppi_editor.setSelectionScheme = function () {
    $("#graph_selection").html("");
    if (tremppi_editor.current_selection.type === "graph") {
        var data = [{id: "0", values: {
                    Edges: tremppi_editor.graph.elements("edge").length,
                    Nodes: tremppi_editor.graph.elements("node").length}}];
    } else {
        var element = tremppi_editor.graph.$("#" + tremppi_editor.current_selection.id);
        var data = [{id: "0", values: element.data()}];
        tremppi_editor.graph.elements("").unselect();
        tremppi_editor.graph.$("#" + element.id()).select();
    }
    var selection = new EditableGrid("selection_" + tremppi_editor.current_selection.id);
    selection.load({"metadata": tremppi_editor.metadata[tremppi_editor.current_selection.type], "data": data});
    selection.renderGrid("graph_selection", "selection");
    selection.modelChanged = tremppi_editor.elementChanged;
};

tremppi_editor.tapFunction = function (event) {
    // Click outside of all components
    if (event.cy == event.cyTarget) {
        // Create new component
        if (tremppi_editor.activity_type === "create") {
            var new_node = {
                group: "nodes",
                data: {
                    Name: "New",
                    MaxActivity: 1,
                    Label: "New:1"
                },
                position: event.cyPosition
            };
            var new_node = tremppi_editor.graph.add(new_node);
            tremppi_editor.current_selection.type = "node";
            tremppi_editor.current_selection.id = new_node.id();
        }
        // Deselect
        else {
            tremppi_editor.current_selection.type = "graph";
            tremppi_editor.current_selection.id = "";
        }
        tremppi_editor.activity_type = "selection";
    } else if (event.cyTarget.isEdge()) {
        // delete edge
        if (tremppi_editor.activity_type === "delete") {
            tremppi_editor.graph.remove(tremppi_editor.graph.$("#" + event.cyTarget.id()));
            tremppi_editor.current_selection.type = "graph";
            tremppi_editor.current_selection.id = "";
        }
        // Select edge
        else if (tremppi_editor.activity_type === "selection") {
            tremppi_editor.current_selection.type = "edge";
            tremppi_editor.current_selection.id = event.cyTarget.id();
        }
    } else if (event.cyTarget.isNode()) {
        var last_node = tremppi_editor.current_selection.id;
        tremppi_editor.current_selection.type = "node";
        tremppi_editor.current_selection.id = event.cyTarget.id();
        // Start an edge
        if (tremppi_editor.activity_type === "create") {
            tremppi_editor.activity_type = "end_regulation";
        }
        // Finish an edge
        else if (tremppi_editor.activity_type === "end_regulation") {
            tremppi_editor.graph.add({
                group: "edges",
                data: {
                    source: last_node,
                    target: tremppi_editor.current_selection.id,
                    Threshold: 1,
                    Label: "tt",
                    line_style: "dashed",
                    line_color: "black",
                    target_arrow_shape: "circle"
                }
            });
            tremppi_editor.activity_type = "selection";
        }
        // delete node
        if (tremppi_editor.activity_type === "delete") {
            tremppi_editor.graph.remove(tremppi_editor.graph.$("#" + event.cyTarget.id()));
            tremppi_editor.saveGraph();
            tremppi_editor.current_selection.type = "graph";
            tremppi_editor.current_selection.id = "";
        }
    }
    tremppi_editor.setControl();
    tremppi_editor.setSelectionScheme();
    tremppi_editor.saveGraph();
};