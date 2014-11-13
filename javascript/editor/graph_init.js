tremppi_editor.graph_init = function() { // on dom ready
    var selected_col = '#0000AA';

    $('#graph_object').cytoscape({
        layout: {
            name: tremppi_common.hasAllPositions(editor.nodes) ? 'preset' : 'grid'
        },
        elements: editor,
        ready: function() {
            var PADDING = 15;
            this.fit(this.filter('node'), PADDING);
        },
        selectionType: "single"
    });

    tremppi_editor.changeVisuals();
    var graph = $("#graph_object").cytoscape('get');
    graph.style().selector('node')
                .css({
                    'content': 'data(Label)',
                    'text-valign': 'center',
                    'background-color': 'white',
                    'border-color': 'black',
                    'border-width': 1,
                    'shape': 'rectangle',
                    'width': 100,
                    'height': 25,
                    'font-size': 15
                })
                .selector('edge')
                .css({
                    'target-arrow-shape': 'triangle',
                    'content': 'data(Threshold)',
                    'text-outline-color': 'black',
                    'text-outline-width': 1,
                    'color': 'white',
                    'target-arrow-color': 'black',
                    'line-color': 'data(line_color)',
                    'line-style': 'data(line_style)',
                    'target-arrow-shape': 'data(target_arrow_shape)',
                    'target-arrow-color': 'data(line_color)',
                    'width': 2
                }).selector(':selected')
                .css({
                    'border-color': selected_col,
                    'line-color': selected_col,
                    'target-arrow-color': selected_col
                }).update();
    tremppi_editor.graph = graph;
    var nodes = graph.elements("node");
    var edges = graph.elements("edge");

    var clickFunction = function(type, id) {
        return function() {
            tremppi_editor.current_selection = {type: type, name: id};
            tremppi_editor.setControlScheme("selection");
        };
    };

    // Set node drag reactions to all
    for (var j = 0; j < nodes.length; j++) 
        var selector = '#' + nodes[j].id();

    graph.on('tap', tremppi_editor.tapFunction);

    tremppi_editor.activity_type = "selection";
    tremppi_editor.current_selection = {type: "graph", name: "regulatory", id: ""};

    // Create metadata for selections
    tremppi_editor.metadata.node = [];
    tremppi_editor.metadata.node.push({name: "Name", label: "NAME", datatype: "string", editable: true});
    tremppi_editor.metadata.node.push({name: "MaxActivity", label: "MAX ACTIVITY", datatype: "integer", editable: true});
    tremppi_editor.metadata.node.push({name: "Constraint", label: "CONSTRAINT", datatype: "string", editable: true});  
    
    tremppi_editor.metadata.edge = [];
    tremppi_editor.metadata.edge.push({name: "Threshold", label: "THRESHOLD", datatype: "integer", editable: true});
    tremppi_editor.metadata.edge.push({name: "Label", label: "LABEL", datatype: "string", editable: true});
    tremppi_editor.metadata.edge[1].values = {
        "+": "Activating", 
        "+&!-": "Activating Only",
        "!-": "Not Inhibiting",
        "-": "Inhibiting", 
        "-&!+": "Inhibiting Only",
        "!+": "Not Activating",
        "+|-": "Observable",
        "!(-|+)": "Not Observable",
        "!(-&+)": "Monotone",
        "-&+": "Not Monotone",
        "(-&!+)|(+&!-)": "Monotone Observable",
        "tt": "Free"
        // ff not added
        // equiv not added
        // +|!- , -|!+ not added
    };
    
    tremppi_editor.metadata.graph = [];
    tremppi_editor.metadata.graph.push({name: "Nodes", label: "NODES", datatype: "integer", editable: false});
    tremppi_editor.metadata.graph.push({name: "Edges", label: "EDGES", datatype: "integer", editable: false});
};