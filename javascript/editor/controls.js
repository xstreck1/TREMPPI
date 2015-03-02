/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

// Create metadata for selections
tremppi.editor.metadata = {
    node: [
        {name: "Name", label: "NAME", datatype: "string", editable: true},
        {name: "MaxActivity", label: "MAX ACTIVITY", datatype: "integer", editable: true},
        {name: "Constraint", label: "CONSTRAINT", datatype: "string", editable: true}
    ],
    edge: [
        {name: "Threshold", label: "THRESHOLD", datatype: "integer", editable: true},
        {name: "Label", label: "LABEL", datatype: "string", editable: true, values: {
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
                // "ff" not added
                // "(-&+)|(!+&!-)" not added
                // "+|!-" 
                // "-|!+" not added
            }
        }],
    graph: [
        {name: "Nodes", label: "NODES", datatype: "integer", editable: false},
        {name: "Edges", label: "EDGES", datatype: "integer", editable: false}
    ]
};

tremppi.editor.elementChanged = function (row_id, column_id, old_val, new_val, row) {
    var element = tremppi.editor.graph.$("#" + tremppi.editor.current_selection.id);
    var val_name = tremppi.editor.metadata[tremppi.editor.current_selection.type][column_id].name;
    element.data(val_name, new_val);
    tremppi.data = tremppi.editor.graph.json().elements;
    tremppi.save();
    tremppi.editor.addValues(tremppi.editor.graph);
    tremppi.editor.graph.style().update();
};

tremppi.editor.setButtons = function () {
    var setHelp = function (text) {
        $("#component_manipulation").append('<span id="graph_help" class="help">' + text + '</div>');
    };
    $("#component_manipulation").html("");
    if (tremppi.editor.activity_type === "selection") {
        $("#component_manipulation").append('<button id="create">Create</button> ELEMENT / ');
        $("#create").click(function (event) {
            tremppi.editor.activity_type = "create";
            tremppi.editor.setButtons();
        });
        $("#component_manipulation").append('<button id="delete">Delete</button> ELEMENTS');
        $("#delete").click(function (event) {
            tremppi.editor.activity_type = "delete";
            tremppi.editor.setButtons();
        });
    } else if (tremppi.editor.activity_type === "create") {
        setHelp("Click on empty space to create a COMPONENT. Click on a component to put a source of a REGULATION.");
    } else if (tremppi.editor.activity_type === "delete") {
        setHelp("Click on a COMPONENT or a REGULATION to delele it. Click on the plane to stop deleting.");
    } else if (tremppi.editor.activity_type === "end_regulation") {
        setHelp("Click on a component to put a target of a REGULATION.");
    }
};

tremppi.editor.setSelectionScheme = function () {
    $("#graph_selection").html("");
    if (tremppi.editor.current_selection.type === "graph") {
        var data = [{id: "0", values: {
                    Edges: tremppi.data.nodes.length,
                    Nodes: tremppi.data.edges.length}}];
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