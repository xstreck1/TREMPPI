tremppi.editor.getByID = function (type, id) {
    var list = elements[type];
    for (var i = 0; i < list.length; i++) {
        if (list[i].data.id === id)
            return list[i];
    }
};

tremppi.editor.elementChanged = function (row_id, column_id, old_val, new_val, row) {
    var element = tremppi.editor.graph.$("#" + tremppi.editor.current_selection.id);
    var val_name = tremppi.editor.metadata[tremppi.editor.current_selection.type][column_id].name;
    element.data(val_name, new_val);
    tremppi.data = tremppi.editor.graph.json().elements;
    tremppi.common.save();
    tremppi.editor.changeVisuals();
};

tremppi.editor.changeVisuals = function() {
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
};

tremppi.editor.setSelectionScheme = function () {
    $("#graph_selection").html("");
    if (tremppi.editor.current_selection.type === "graph") {
        var data = [{id: "0", values: {
                    Edges: tremppi.editor.graph.elements("edge").length,
                    Nodes: tremppi.editor.graph.elements("node").length}}];
    } else {
        var element = tremppi.editor.graph.$("#" + tremppi.editor.current_selection.id);
        var data = [{id: "0", values: element.data()}];
        tremppi.editor.graph.elements("").unselect();
        tremppi.editor.graph.$("#" + element.id()).select();
    }
    var selection = new EditableGrid("selection_" + tremppi.editor.current_selection.id);
    selection.load({"metadata": tremppi.editor.metadata[tremppi.editor.current_selection.type], "data": data});
    selection.renderGrid("graph_selection", "selection");
    selection.modelChanged = tremppi.editor.elementChanged;
};

tremppi.editor.tapFunction = function (event) {
    console.log("tap");
    // Click outside of all components
    if (event.cy == event.cyTarget) {
        // Create new component
        if (tremppi.editor.activity_type === "create") {
            var new_node = {
                group: "nodes",
                data: {
                    Name: "New",
                    MaxActivity: 1,
                    Label: "New:1"
                },
                position: event.cyPosition
            };
            var new_node = tremppi.editor.graph.add(new_node);
            tremppi.editor.current_selection.type = "node";
            tremppi.editor.current_selection.id = new_node.id();
        }
        // Deselect
        else {
            tremppi.editor.current_selection.type = "graph";
            tremppi.editor.current_selection.id = "";
        }
        tremppi.editor.activity_type = "selection";
    } else if (event.cyTarget.isEdge()) {
        // delete edge
        if (tremppi.editor.activity_type === "delete") {
            tremppi.editor.graph.remove(tremppi.editor.graph.$("#" + event.cyTarget.id()));
            tremppi.editor.current_selection.type = "graph";
            tremppi.editor.current_selection.id = "";
        }
        // Select edge
        else if (tremppi.editor.activity_type === "selection") {
            tremppi.editor.current_selection.type = "edge";
            tremppi.editor.current_selection.id = event.cyTarget.id();
        }
    } else if (event.cyTarget.isNode()) {
        var last_node = tremppi.editor.current_selection.id;
        tremppi.editor.current_selection.type = "node";
        tremppi.editor.current_selection.id = event.cyTarget.id();
        // Start an edge
        if (tremppi.editor.activity_type === "create") {
            tremppi.editor.activity_type = "end_regulation";
        }
        // Finish an edge
        else if (tremppi.editor.activity_type === "end_regulation") {
            tremppi.editor.graph.add({
                group: "edges",
                data: {
                    source: last_node,
                    target: tremppi.editor.current_selection.id,
                    Threshold: 1,
                    Label: "tt",
                    line_style: "dashed",
                    line_color: "black",
                    target_arrow_shape: "circle"
                }
            });
            tremppi.editor.activity_type = "selection";
        }
        // delete node
        if (tremppi.editor.activity_type === "delete") {
            tremppi.editor.graph.remove(tremppi.editor.graph.$("#" + event.cyTarget.id()));
            tremppi.editor.current_selection.type = "graph";
            tremppi.editor.current_selection.id = "";
        }
    }
    tremppi.editor.setControls();
    tremppi.editor.setSelectionScheme();
    tremppi.data = tremppi.editor.graph.json().elements;
    tremppi.common.save();
};