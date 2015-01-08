var tremppi = {
    common: {
        server_port: 8080,
        server_location: "localhost",
        server_available: false,
        getServerAddress: function () {
            return "http://" + tremppi.common.server_location + ":" + tremppi.common.server_port + "/";
        },
        // the setup function - sets globals, obtains data and adds server content, if possible
        set: function (widget_name, file_name) {
            var self = tremppi.common;
            tremppi[widget_name] = {};
            tremppi.common.widge = widget_name;
            tremppi.common.data_file = widget_name + file_name + ".json";
            $(window).ready(
                    $.getJSON(self.data_file,
                            function (data) {
                                // load data
                                tremppi.data = data;
                                tremppi[widget_name].load();
                                // set a function that will set the server functionality
                                self.addServerContent(tremppi[widget_name].setServer);
                            }
                    )
                    );
        },
        // Saves the variable to the respective file
        save: function () {
            if (!tremppi.common.server_available)
                return;

            var content = JSON.stringify(tremppi.data, null, '\t');
            var url = tremppi.common.getServerAddress() + "?" + tremppi.common.data_file;
            $.ajax({
                type: "POST",
                url: url,
                data: content,
                success: function (res) {
                }
            });
        },
        // Executes the provided function in the case the server is running
        addServerContent: function (content_function) {
            $.ajax(
                    {url: tremppi.common.getServerAddress(),
                        success: function (result) {
                            tremppi.common.server_available = true;
                            content_function();
                        },
                        error: function (jqXHR, textStatus, errorThrown) {
                            tremppi.common.server_available = false;
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
        },
        // test if nodes all have positions
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
        }
    }
};