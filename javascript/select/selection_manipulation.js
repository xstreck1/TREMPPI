tremppi_select.loadSelections = function () {
    $("#selection_holder").empty();
    var selections_grid = new EditableGrid("selections");
    
    if (select.length === 0) {
        select = [{
            id: 0,
            values: {
                Name: "",
                Selection: "",
                Select: false,
                Compare: false
            }
        }];
    }

    selections_grid.load({"metadata": tremppi_select.metadata["selections"], "data": select});
    selections_grid.renderGrid("selection_holder", "selections");
    selections_grid.modelChanged = tremppi_select.selectionChanged;

    if (selections_grid.data.length === 0 || !tremppi_common.isEmpty(selections_grid.data[selections_grid.data.length - 1].columns))
        selections_grid.append(selections_grid.data.length, {
            values: {
                Name: "",
                Selection: "",
                Select: false,
                Compare: false
            }
        });
};

// Called if the data change
tremppi_select.selectionChanged = function (row_id, colum_id, old_val, new_val, row) {
    var type = this.name.split("_")[0];
    var index = this.name.split("_")[1];
    var column_name = this.columns[colum_id].name;

    // In data, update the row count, if necessary
    var columns = this.data[row_id].columns;
    // If is not last and is empty
    if (tremppi_common.isEmpty(columns) && row_id !== (this.data.length - 1))
        this.remove(row_id);
    else if (!tremppi_common.isEmpty(columns) && row_id === (this.data.length - 1))
        this.append(this.data.length, new Array(this.columns.length), true, true);

    // Store the data
    var row_count = this.data.length;

    select = new Array(row_count);
    for (var row_i = 0; row_i < row_count; row_i++) {
        select[row_i] = {};
        select[row_i].id = this.data[row_i].originalIndex;
        select[row_i].values = {};
        for (var column_i = 0; column_i < this.columns.length; column_i++) {
            var column_name = this.columns[column_i].name;
            select[row_i].values[column_name] = this.data[row_i].columns[column_i];
        }
    }
    
    
    tremppi_common.save("select");
};