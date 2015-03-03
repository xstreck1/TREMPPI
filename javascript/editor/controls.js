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

tremppi.editor.graphChanged = function () {
    tremppi.editor.addValues(tremppi.editor.graph);
    tremppi.editor.graph.style().update();
    tremppi.data = tremppi.editor.graph.json().elements;
    tremppi.save();
};

// calls appropriate controls handler
tremppi.editor.setControls = function () {
    $("#controls").html("");
    tremppi.editor[tremppi.editor.current_selection.type + "Controls"]();
};

tremppi.editor.addHelpField = function (text) {
    $("#controls").append('<span id="graph_help" class="help">' + text + '</div>');
};

tremppi.editor.addControlButton = function (btn_function) {
    $("#controls").append('<button id="' + btn_function + '_btn" class="btn">' + btn_function + ' element</button>');
    $("#" + btn_function + "_btn").click(function (event) {
        tremppi.editor.activity_type = btn_function;
        tremppi.editor.setControls();
    });
};

tremppi.editor.graphControls = function () {
    if (tremppi.editor.activity_type === "selection") {
        tremppi.editor.addControlButton("create");
        tremppi.editor.addControlButton("delete");
    } else if (tremppi.editor.activity_type === "create") {
        tremppi.editor.addHelpField("Click on empty space to create a COMPONENT. Click on a component to put a source of a REGULATION.");
    } else if (tremppi.editor.activity_type === "delete") {
        tremppi.editor.addHelpField("Click on a COMPONENT or a REGULATION to delele it. Click on the plane to stop deleting.");
    }
};

tremppi.editor.addEditField = function (element, field) {
    $("#controls").append('<input id="' + field + '_input" placeholder="' + field + '" value="' + element.data(field) + '"/>');
    return $("#" + field + "_input").change(function () {
        element.data(field, this.value);
        tremppi.editor.graphChanged();
    });
};

tremppi.editor.edgeControls = function () {
    var element = tremppi.editor.graph.$("#" + tremppi.editor.current_selection.id);

    tremppi.editor.addEditField(element, "Threshold").w2field('int');
    tremppi.editor.addEditField(element, "Label").w2field('list', {
        items: tremppi.editor.lables,
        selected: element.data("Label")
    });
};

tremppi.editor.nodeControls = function () {
    var element = tremppi.editor.graph.$("#" + tremppi.editor.current_selection.id);

    if (tremppi.editor.activity_type === "end_regulation") {
        tremppi.editor.addHelpField("Click on a component to put a target of a REGULATION.");
    } else {
        tremppi.editor.addEditField(element, "Name").w2field('text');
        tremppi.editor.addEditField(element, "MaxActivity").w2field('int');
    }
};