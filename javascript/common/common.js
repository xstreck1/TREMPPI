var tremppi_common = {
    // Saves the variable to the respective file
    save: function (var_name) {
        var content = "var " + var_name + " = " + JSON.stringify(window[var_name], null, '\t') + ";";
        var url = tremppi_common.server_address + "?" + tremppi_common.data_path + var_name + ".js";
        $.ajax({
            type: "POST",
            url: url,
            data: content,
            success: function (res) {
            }
        });
    },
    hasAllPositions: function (nodes) {
        if (typeof nodes === 'undefined')
            return true;
        for (var i = 0; i < nodes.length; i++) {
            if (!nodes[i].position)
                return false;
            if (!nodes[i].position.x || !nodes[i].position.y)
                return false;
        }
        return true;
    },
    // Executes the provided function in the case the server is running
    addServerContent: function (content_function) {
        $.ajax(
                {url: tremppi_common.server_address,
                    success: function (result) {
                        content_function();
                    },
                    error: function (jqXHR, textStatus, errorThrown) {
                        console.log("Server not available");
                    }
                }
        );
    },
    // test is a row is not empty
    isEmpty: function (columns) {
        for (var i = 0; i < columns.length; i++) {
            if (columns[i] !== "" && columns[i] !== false)
                return false;
        }
        return true;
    }
};