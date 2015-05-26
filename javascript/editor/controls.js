/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi */

tremppi.editor.controls = function () {
    tremppi.editor.graph.on('tapend', tremppi.editor.selection);
    // Save after drag

    tremppi.toolbar.onClick = tremppi.toolbarClick;
    tremppi.editor.setBasic();
};

tremppi.toolbarClick = function (event) {
    tremppi.editor[event.target]();
};

tremppi.editor.save = function () {
    var data = tremppi.editor.graph.json().elements;
    tremppi.saveData(data);
};

tremppi.editor.create = function () {
    tremppi.editor.removeAll();
    tremppi.editor.addHelpField("Click on empty space to create a COMPONENT. Click on a component to put a source of a REGULATION.");
    tremppi.editor.graph.off('tapend').on('tapend', tremppi.editor.creation);
};

tremppi.editor.delete = function () {
    tremppi.editor.removeAll();
    tremppi.editor.addHelpField("Click on a COMPONENT or a REGULATION to delele it.");
    tremppi.editor.graph.off('tapend').on('tapend', tremppi.editor.deletion);
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

tremppi.editor.addElem = function (elem, type) {
    tremppi.editor["glyph" + type](elem.data);
    var new_elem = tremppi.editor.graph.add(elem)[0];
    tremppi.editor["set" + type](new_elem);
    tremppi.editor.graph.off('tapend').on('tapend', tremppi.editor.selection);
    tremppi.editor.graph.style().update();
};

tremppi.editor.creation = function (event) {
    if (event.cy === event.cyTarget) {
        var node = tremppi.editor.newNode(event.cyPosition);
        tremppi.editor.addElem(node, "Node");
    }
    else if (event.cyTarget.isNode()) {
        var source = event.cyTarget[0];
        tremppi.editor.removeAll();
        tremppi.editor.addHelpField("Click on a component to put a target of a REGULATION.");
        tremppi.editor.graph.off('tapend').on('tapend', function (event) {
            if (event.cy !== event.cyTarget && event.cyTarget.isNode()) {
                var edge = tremppi.editor.newEdge(source, event.cyTarget[0]);
                tremppi.editor.addElem(edge, "Edge");
            } else {
                tremppi.editor.graph.off('tapend').on('tapend', tremppi.editor.selection);
            }
        });
    } else {
        tremppi.editor.graph.off('tapend').on('tapend', tremppi.editor.selection);
    }
};

tremppi.editor.deletion = function (event) {
    if (event.cy !== event.cyTarget) {
        tremppi.editor.graph.remove(event.cyTarget);
        tremppi.editor.graph.style().update();
    }
    tremppi.editor.setBasic();
};

tremppi.editor.removeAll = function () {
    // Destroy all the elements
    while (tremppi.toolbar.items.length !== 0) {
        tremppi.toolbar.remove(tremppi.toolbar.items[0].id);
    }
};

tremppi.editor.addEditField = function (element, field, type) {
    tremppi.toolbar.add({type: 'html', id: field, html:
                '<input id="' + field + '_input" type="' + type + '"placeholder="' + field + '"/>'
    });
    $("#" + field + "_input").val(element.data(field)).change(function () {
        var value = this.type === "number" ? parseInt(this.value) : this.value;
        element.data(field, value);
        tremppi.editor[element[0].isNode() ? 'glyphNode' : 'glyphEdge'](element.data());
        tremppi.editor.graph.style().update();
    }).height(14);
};

tremppi.editor.addHelpField = function (help_text) {
    tremppi.toolbar.add({type: 'html', id: "Help", html: '<div id="Help">' + help_text + '</div>'});
};

tremppi.editor.setBasic = function () {
    tremppi.editor.removeAll();
    tremppi.toolbar.add({type: 'button', id: 'save', caption: 'Save', img: 'icon-page', hint: 'Save the data'});
    tremppi.toolbar.add({type: 'button', id: 'create', icon: 'w2ui-icon-plus', caption: 'Add', hint: 'click on an empty space to create a node, click on a node to start an edge'});
    tremppi.toolbar.add({type: 'button', id: 'delete', icon: 'w2ui-icon-cross', caption: 'Delete', hint: 'delete an element'});
    tremppi.editor.graph.off('tapend').on('tapend', tremppi.editor.selection);
};

tremppi.editor.setNode = function (node) {
    tremppi.editor.removeAll();
    tremppi.editor.addEditField(node, "Name", "text");
    $("#Name_input").w2field('text');
    tremppi.editor.addEditField(node, "MaxActivity", "number");
    $("#MaxActivity_input").w2field('int');
};

tremppi.editor.setEdge = function (edge) {
    tremppi.editor.removeAll();
    tremppi.editor.addEditField(edge, "Threshold", "number");
    $("#Threshold_input").w2field('int');
    tremppi.editor.addEditField(edge, "Label", "text");
    $("#Label_input").w2field('list', {
        items: tremppi.editor.lables,
        selected: edge.data("Label")
    });
    $("#Label_input").val(edge.data("Label"));
};