/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.editor.setServer = function () {
    var setHelp = function (text) {
        $("#control_panel").append('<div id="graph_help" class="help">' + text + '</div>');
    };
    $("#control_panel").html("");
    if (tremppi.editor.activity_type === "selection") {
        $("#control_panel").append('<div id="graph_control" ></div>');
        $("#graph_control").append('<button id="create">Create</button> ELEMENT / ');
        $("#create").click(function (event) {
            tremppi.editor.activity_type = "create";
            tremppi.editor.setServer();
        });
        $("#graph_control").append('<button id="delete">Delete</button> ELEMENTS');
        $("#delete").click(function (event) {
            tremppi.editor.activity_type = "delete";
            tremppi.editor.setServer();
        });
    } else if (tremppi.editor.activity_type === "create") {
        setHelp("Click on empty space to create a COMPONENT. Click on a component to put a source of a REGULATION.");
    } else if (tremppi.editor.activity_type === "delete") {
        setHelp("Click on a COMPONENT or a REGULATION to delele it. Click on the plane to stop deleting.");
    } else if (tremppi.editor.activity_type === "end_regulation") {
        setHelp("Click on a component to put a target of a REGULATION.");
    }

    $("#control_pad").height(($("#control_panel").height() + 20) + "px");
};
