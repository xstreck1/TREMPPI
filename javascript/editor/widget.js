/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.editor.setStyle = function (graph) {
    var selected_col = '#0000AA';

    graph.style().selector('node').css({
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

tremppi.editor.changeVisuals = function (graph) {
    var edges = graph.elements("edge");
    for (var i = 0; i < edges.length; i++) {
        var data = edges[i].data();
        var label = data.Label;
        if (label === "+" || label === "-" || label === "+&!-" || label === "-&!+"
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
    var nodes = tremppi.editor.graph.elements("node");
    for (var i = 0; i < nodes.length; i++) {
        var node_data = nodes[i].data();
        node_data.Label = node_data.Name + ":" + node_data.MaxActivity;
    }
};

tremppi.editor.tapFunction = function (event) {
    console.log("tap");
    // Click outside of all components
    if (event.cy === event.cyTarget) {
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
    tremppi.editor.setSelectionScheme();
    tremppi.editor.setButtons();
    tremppi.data = tremppi.editor.graph.json().elements;
    tremppi.save();
};