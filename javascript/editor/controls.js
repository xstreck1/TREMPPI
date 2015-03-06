/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.editor.toolbarClick = function (event) {
    tremppi.editor[event.target]();
};

tremppi.editor.Create = function () {
    tremppi.editor.removeAll();
    tremppi.editor.addHelpField("Click on empty space to create a COMPONENT. Click on a component to put a source of a REGULATION.");
    tremppi.editor.graph.off('tap').on('tap', tremppi.editor.creation);
};

tremppi.editor.Delete = function () {
    tremppi.editor.removeAll();
    tremppi.editor.addHelpField("Click on a COMPONENT or a REGULATION to delele it.");
    tremppi.editor.graph.off('tap').on('tap', tremppi.editor.deletion);
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

tremppi.editor.creation = function (event) {
    if (event.cy === event.cyTarget) {
        var new_node = tremppi.editor.graph.add(tremppi.editor.newNode(event.cyPosition))[0];
        tremppi.editor.addValues(tremppi.editor.graph);
        tremppi.editor.graphChanged();
        tremppi.editor.setNode(new_node);

        tremppi.editor.graph.off('tap').on('tap', tremppi.editor.selection);
    }
    else if (event.cyTarget.isNode()) {
        var source = event.cyTarget[0];
        tremppi.editor.removeAll();
        tremppi.editor.addHelpField("Click on a component to put a target of a REGULATION.");
        tremppi.editor.graph.off('tap').on('tap', function (event) {
            tremppi.editor.newEdge(source, event.cyTarget[0]);
        });
        tremppi.editor.graph.off('tap').on('tap', tremppi.editor.selection);
    } else {
        tremppi.editor.setEdge(new_node);
        tremppi.editor.graph.off('tap').on('tap', tremppi.editor.selection);
    }
};

tremppi.editor.deletion = function (event) {
    if (event.cy !== event.cyTarget) {
        tremppi.editor.graph.remove(event.cyTarget);
    }
    tremppi.editor.setBasic();
};

tremppi.editor.graphChanged = function () {
    tremppi.editor.addValues(tremppi.editor.graph);
    tremppi.editor.graph.style().update();
    tremppi.editor.save();
};

tremppi.editor.removeAll = function () {
    // Destroy all the elements
    while (tremppi.editor.toolbar.items.length !== 0) {
        tremppi.editor.toolbar.remove(tremppi.editor.toolbar.items[0].id);
    }
};

tremppi.editor.addEditField = function (element, field) {
    tremppi.editor.toolbar.add({type: 'html', id: field, html: '<input id="' + field + '_input" placeholder="' + field + '"/>'});
    $("#" + field + "_input").val(element.data(field)).change(function () {
        element.data(field, this.value);
        tremppi.editor.graphChanged();
    }).height(14);
};

tremppi.editor.addHelpField = function (help_text) {
    tremppi.editor.toolbar.add({type: 'html', id: "Help", html: '<div id="Help">' + help_text + '</div>'});
};

tremppi.editor.setBasic = function () {
    tremppi.editor.removeAll();
    tremppi.editor.toolbar.add({type: 'button', id: 'Create', caption: 'Create', hint: 'hint create'});
    tremppi.editor.toolbar.add({type: 'button', id: 'Delete', caption: 'Delete', hint: 'hint delete'});
    tremppi.editor.graph.off('tap').on('tap', tremppi.editor.selection);
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
        selected: edge.data("Label")
    });
    $("#Label_input").val(edge.data("Label"));
};