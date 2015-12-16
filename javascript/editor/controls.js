/******************************************************************************
Created by Adam Streck, 2013-2015, adam.streck@fu-berlin.de

This file is part of the Toolkit for Reverse Engineering of Molecular Pathways
via Parameter Identification (TREMPPI)

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

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
    tremppi.editor.addEditField(node, "Constraint", "text");
    $("#Constraint_input").w2field('text');
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