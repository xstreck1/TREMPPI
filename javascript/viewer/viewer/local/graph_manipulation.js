tremppi_viewer.positionNodes = function(graph) {
    for (var i = 0; i < elements.nodes.length; i++) {
        var pos = {};
        if (elements.nodes[i].x)
            pos.x = elements.nodes[i].x;
        if (elements.nodes[i].y)
            pos.y = elements.nodes[i].y;
        var filter = 'node[id = "' + elements.nodes[i].data.id + '"]';
        var selected = graph.filter(filter);
        selected.position(pos);
    }
};

tremppi_viewer.getByID = function(type, id) {
    var list = elements[type];
    for (var i = 0; i < list.length; i++) {
        if (list[i].data.id === id)
            return list[i];
    }
};

tremppi_viewer.setControlScheme = function(scheme_type) {
    $("#graph_control").html("");
    if (scheme_type === "default") {
        $("#graph_control").append('<button id="new_component">New Component</button>');
        $("#new_component").click(function(event) {
            tremppi_viewer.activity_type = "new_component";
        });
        $("#graph_control").append('<button id="new_regulation">New Regulation</button>');
        $("#new_component").click(function(event) {
            tremppi_viewer.activity_type = "new_regulation";
        });
    } else if (scheme_type === "selection") {
        $("#graph_control").append("id: " + tremppi_viewer.current_selection.name + ", type: " + tremppi_viewer.current_selection.type);
    }
};

tremppi_viewer.tapFunction = function(event) {
    if (event.cy == event.cyTarget) {
        tremppi_viewer.current_selection.type = "graph";
        tremppi_viewer.current_selection.name = "regulatory";
    } else if (event.cyTarget.isEdge()) {
        tremppi_viewer.current_selection.type = "edge";
        tremppi_viewer.current_selection.name = event.cyTarget.id();
    } else if (event.cyTarget.isNode()) {
        tremppi_viewer.current_selection.type = "node";
        tremppi_viewer.current_selection.name = event.cyTarget.id();
    }
    tremppi_viewer.setControlScheme("selection");
}