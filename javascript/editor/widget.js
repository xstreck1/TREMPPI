/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.editor.lables = [
    "Activating",
    "Activating Only",
    "Not Inhibiting",
    "Inhibiting",
    "Inhibiting Only",
    "Not Activating",
    "Observable",
    "Not Observable",
    "Monotone",
    "Not Monotone",
    "Monotone Observable",
    "Free"
            // "ff" not added
            // "(-&+)|(!+&!-)" not added
            // "+|!-" 
            // "-|!+" not added
];

tremppi.editor.setStyle = function (graph) {
    var selected_col = "#FFA500";

    graph.style().selector('node').css({
        'content': 'data(Label)',
        'text-valign': 'center',
        'color': 'black',
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
        'color': selected_col
    }).update();
};

tremppi.editor.addValues = function (graph) {
    var edges = graph.elements("edge");
    for (var i = 0; i < edges.length; i++) {
        var data = edges[i].data();
        var label = data.Label;
        if (label === "Activating" || label === "Inhibiting" || label === "Activating Only" || label === "Inhibiting Only"
                || label === "Observable" || label === "Not Monotone" || label === "Monotone Observable")
            data.line_style = "solid";
        else
            data.line_style = "dashed";
        if (label === "Not Inhibiting" || label === "Activating Only")
            data.line_color = "green";
        else if (label === "Not Activating" || label === "Inhibiting Only")
            data.line_color = "red";
        else
            data.line_color = "black";
        if (label === "Activating" || label === "Activating Only")
            data.target_arrow_shape = "triangle";
        else if (label === "Inhibiting" || label === "Inhibiting Only")
            data.target_arrow_shape = "tee";
        else
            data.target_arrow_shape = "circle";
    }
    var nodes = graph.elements("node");
    for (var i = 0; i < nodes.length; i++) {
        var node_data = nodes[i].data();
        node_data.Label = node_data.Name + ":" + node_data.MaxActivity;
    }
};

tremppi.editor.newNode = function (click_position) {
    return {
        group: "nodes",
        data: {
            Name: "New",
            MaxActivity: 1
        },
        position: click_position
    };
};

tremppi.editor.newEdge = function (source, target) {
    return {
        group: "edges",
        data: {
            source: source.id(),
            target: target.id(),
            Threshold: 1,
            Label: "Free"
        }
    };
};
