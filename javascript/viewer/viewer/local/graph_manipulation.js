tremppi_viewer.positionNodes = function(graph) {
    for (var i = 0; i < elements.nodes.length; i++) {
        var pos = {};
        if (elements.nodes[i].x)
            pos.x = elements.nodes[i].x;
        if (elements.nodes[i].y)
            pos.y = elements.nodes[i].y;
        var filter = 'node[id = "' + elements.nodes[i].data.id + '"]'
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