/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.select.metadata = {
    
    selections: [
        {name: "Selection", label: "WHERE", datatype: "string", editable: true},
        {name: "Select", label: "SELECT", datatype: "boolean", editable: true},
        {name: "Compare", label: "COMPARE", datatype: "boolean", editable: true}
    ]
};

tremppi.select.emptyRow = function () {
    return {
        Selection: "",
        Select: false,
        Compare: false
    };
};

tremppi.select.loadSelections = function () {
    $("#selection_table").empty();
    var selections_grid = new EditableGrid("Selection Table");
    selections_grid.load({metadata: tremppi.select.metadata["selections"], data: tremppi.data});
    selections_grid.renderGrid("selection_table", "selection");
};

tremppi.select.setTable = function () {
    // Adds non-zero counts of elements
    var set_counts = function (type) {
        $.ajax({
            url: tremppi.common.getServerAddress() + "?command=counts&pick=" + type,
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
            url: tremppi.common.getServerAddress() + "?command=rows&pick=" + type + "&count=" + tremppi.select.row_count,
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
            url: tremppi.common.getServerAddress() + "?command=columns&pick=" + type,
            success: function (result) {
                var col_names = result.split(",");
                var table_content = '<tr ><th class="header" colspan="' + (col_names.length + 1) + '">' + type + "</th></tr>";
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
        url: tremppi.common.getServerAddress() + "?command=database",
        success: function (result) {
            if (!result) {
                $("#table_holder").append("The database content is empty.");
            } else {
                $("#table_holder").empty();
                $("#table_holder").append('<table id="Select_table" ></table>');
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

// Called if the data change
tremppi.select.selectionChanged = function (row_id, colum_id, old_val, new_val, row) {
    var type = this.name.split("_")[0];
    var index = this.name.split("_")[1];
    var column_name = this.columns[colum_id].name;

    // In data, update the row count, if necessary
    var columns = this.data[row_id].columns;
    // If is not last and is empty
    if (tremppi.common.isEmpty(columns) && row_id !== (this.data.length - 1))
        this.remove(row_id);
    else if (!tremppi.common.isEmpty(columns) && row_id === (this.data.length - 1))
        this.append(this.data.length, new Array(this.columns.length), true, true);

    // Store the data
    var row_count = this.data.length;

    tremppi.data = new Array(row_count);
    for (var row_i = 0; row_i < row_count; row_i++) {
        tremppi.data[row_i] = {};
        tremppi.data[row_i].id = this.data[row_i].originalIndex;
        tremppi.data[row_i].values = {};
        for (var column_i = 0; column_i < this.columns.length; column_i++) {
            var column_name = this.columns[column_i].name;
            tremppi.data[row_i].values[column_name] = this.data[row_i].columns[column_i];
        }
    }

    tremppi.common.save();
};