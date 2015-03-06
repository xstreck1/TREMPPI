/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.editor.toolbarClick = function (event) {
    console.log('Target: ' + event.target, event);
};

tremppi.editor.graphChanged = function () {
    tremppi.editor.addValues(tremppi.editor.graph);
    tremppi.editor.graph.style().update();
    tremppi.editor.save();
};

tremppi.editor.removeAll = function () {
    // Destroy all the elements
    tremppi.editor.toolbar.remove('Create', 'Delete', 'Name', 'MaxActivity', 'Label', 'Threshold');
};

tremppi.editor.addEditField = function (element, field) {
    tremppi.editor.toolbar.add({type: 'html', id: field, html: '<input id="' + field + '_input" placeholder="' + field + '"/>' });
    $("#" + field + "_input").val(element.data(field)).change(function () {
        element.data(field, this.value);
        tremppi.editor.graphChanged();
    });
};

tremppi.editor.setBasic = function () {
    tremppi.editor.removeAll();
    tremppi.editor.toolbar.add({type: 'button', id: 'Create', caption: 'Create', hint: 'hint create'});
    tremppi.editor.toolbar.add({type: 'button', id: 'Delete', caption: 'Delete', hint: 'hint delete'});
};

tremppi.editor.setNode = function (node) {
    tremppi.editor.removeAll();
    tremppi.editor.addEditField(node, "Name");
    $("#Name_input").w2field('text'); 
    tremppi.editor.addEditField(node, "MaxActivity");
    $("#MaxActivity_input").w2field('int');
};

tremppi.editor.setEdge = function (edge) {
    tremppi.editor.removeAll();
    tremppi.editor.addEditField(edge, "Threshold");
    $("#Threshold_input").w2field('int');
    tremppi.editor.addEditField(edge, "Label");
    $("#Label_input").w2field('list', {
        items: tremppi.editor.lables,
        selected: element.data("Label")
    });
    $("#Label_input").val(edge.data("Label"));
};

tremppi.editor.selection = function (event) {
    if (event.cy === event.cyTarget) {
        tremppi.editor.setBasic();
    }
    else if (event.cyTarget.isNode()) {
        tremppi.editor.setNode(event.cyTarget[0]);
    }
    else if (event.cyTarget.isEdge()) {
        tremppi.editor.setEdge(event.cyTarget[0]);
    }
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

    }
};