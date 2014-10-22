tremppi_select.set_table = function () {
    // Adds data to the tables
    var set_rows = function () {
        $.ajax({
            url: tremppi_common.server_address + "?rows",
            success: function (result) {
                var rows = result.split("\n");
                for (var i = 0; i < rows.length - 1; i++) {
                    var items = rows[i].split(" ");
                    var table_content = "<tr>";

                    for (var j = 0; j < items.length - 1; j++) {
                        table_content += "<td>" + items[j] + "</td>";
                    }

                    table_content += "</tr>";
                    $("#param_table").append(table_content);

                }
            },
            error: function (jqXHR, textStatus, errorThrown) {
                var code = jqXHR.statusCode();
            }
        });
    };

    // Creates the table, then calls function to add data
    $.ajax({
        url: tremppi_common.server_address + "?columns",
        success: function (result) {
            if (!result) {
                $("#table_holder").append("The database content is empty.");
            }
            else {
                $("#table_holder").empty();
                var table_content = '<table id="param_table" ><tr>';
                var res = result.split("\n");
                for (var i = 0; i < res.length; i++) {
                    table_content += "<th>" + res[i] + "</th>";
                }
                table_content += "</tr></table>";
                $("#table_holder").append(table_content);
                set_rows();
            }
        },
        error: function (jqXHR, textStatus, errorThrown) {
            var code = jqXHR.statusCode();
            $("#table_holder").html('ERROR: unable to obtain the database. You must execute tremppi spawn to create it first.');
        }
    });
};