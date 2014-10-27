tremppi_select.set_table = function () {
    // Adds non-zero counts of elements
    var set_counts = function (type) {
        $.ajax({
            url: tremppi_common.server_address + "?command=counts&pick=" + type,
            success: function (result) {
                var columns_counts = result.split(",");

                var table_content = "<tr><th>COUNT</th>";

                for (var j = 0; j < columns_counts.length; j++)
                    table_content += "<th>" + columns_counts[j] + "</th>";

                table_content += "</tr>";
                $("#" + type + "_table").append(table_content);
            }
        });
    };

    // Adds data to the tables
    var set_rows = function (type) {
        $.ajax({
            url: tremppi_common.server_address + "?command=rows&pick=" + type,
            success: function (result) {
                if (result !== "") {
                    var rows = result.split("\n");

                    for (var i = 0; i < rows.length; i++) {
                        var items = rows[i].split(",");
                        var table_content = "<tr><th>ROW</th>";

                        for (var j = 0; j < items.length; j++)
                            table_content += "<td>" + items[j] + "</td>";

                        table_content += "</tr>";
                        $("#" + type + "_table").append(table_content);

                    }
                }
                set_counts(type);
            }
        });
    };

    // Start the cascade for obtaining the table data, initialize with columns
    var make_table = function (type) {
        $.ajax({
            url: tremppi_common.server_address + "?command=columns&pick=" + type,
            success: function (result) {
                var col_names = result.split(",");
                var table_content = '<tr><th colspan="' + (col_names.length + 1) + '">' + type + "</th></tr>";
                table_content += "<tr><th></th>";

                for (var i = 0; i < col_names.length; i++)
                    table_content += "<th>" + col_names[i] + "</th>";

                table_content += "</tr>";
                $("#" + type + "_table").append(table_content);
                set_rows(type);
            }
        });
    };

    // Main building function, displays the selection tables 
    $.ajax({
        url: tremppi_common.server_address + "?command=database",
        success: function (result) {
            if (!result) {
                $("#table_holder").append("The database content is empty.");
            } else {
                $("#table_holder").empty();
                $("#table_holder").append('<table id="Select_table" ></table>');
                $("#table_holder").append('<br />');
                make_table("Select");
                $("#table_holder").append('<table id="Compare_table" ></table>');
                make_table("Compare");
            }
        },
        error: function (jqXHR, textStatus, errorThrown) {
            var code = jqXHR.statusCode();
            $("#table_holder").html('ERROR: unable to obtain the database. You must execute tremppi spawn to create it first.');
        }
    });
};