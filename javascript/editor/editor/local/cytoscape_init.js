tremppi_editor.cytoscape_init = function() { // on dom ready
    var selected_col = '#5555BB';

    $('#graph_object').cytoscape({
        style: cytoscape.stylesheet()
                .selector('node')
                .css({
                    'content': 'data(Name)',
                    'text-valign': 'center',
                    'background-color': 'white',
                    'border-color': 'black',
                    'border-width': 2,
                    'width': 50,
                    'height': 50
                })
                .selector('edge')
                .css({
                    'target-arrow-shape': 'triangle',
                    'content': 'data(score)',
                    'target-arrow-color': 'black',
                    'line-color': 'black',
                    'width': 2.5
                }).selector(':selected')
                .css({
                    'border-color': selected_col,
                    'line-color': selected_col,
                    'target-arrow-color': selected_col
                }),
        layout: {
            name: tremppi_common.hasAllPositions(elements.nodes) ? 'preset' : 'grid',
        },
        elements: elements,
        ready: function() {
            var PADDING = 15;
            this.fit(this.filter('node'), PADDING);
        }
    });

    var graph = $("#graph_object").cytoscape('get');
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
    for (var j = 0; j < nodes.length; j++) {
        var selector = '#' + nodes[j].id();
    }

    graph.on('tap', tremppi_editor.tapFunction);

    tremppi_editor.activity_type = "selection";
    tremppi_editor.current_selection = {type: "graph", name: "regulatory"};

    // Set the control scheme
    tremppi_editor.setButtonControl();

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
        "(!-&!+)": "Not Observable",
        "!(-&+)": "Monotone",
        "-&+": "Not Monotone",
        "(-&!+)|(+&!-)": "Monotone Observable",
        "tt": "Free"
    };
};