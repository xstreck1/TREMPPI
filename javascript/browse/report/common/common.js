var tremppi_common = {
    promptWithContent: function(var_name) {
        var content = "var " + var_name + " = " + JSON.stringify(window[var_name], null, '\t') + ";";
        prompt("Press ctrl+C to copy, then paste to " + var_name + ".js", content);
    },
    hasAllPositions: function(nodes) {
        for (var i = 0; i < nodes.length; i++) {
            if (!nodes[i].position)
                return false;
            if (!nodes[i].position.x || !nodes[i].position.y)
                return false;
        }
        return true;
    }
}