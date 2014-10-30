tremppi_properties.remove = function (index) {
    $("#property_" + index).html("");
    properties.splice(index, 1);
    tremppi_properties.createPropertyList();
};

// Add new property
tremppi_properties.add = function () {
    properties.push({
        "desc": [
            {
                "id": 0,
                "values": {
                    "Name": "property_" + properties.length,
                    "Type": "TimeSeries",
                    "Experiment": "",
                    "Verify": "true"
                }
            }
        ],
        "data": [
            {
                "id": 0,
                "values": {
                    "Measurement": "true",
                }
            }
        ]
    });
    tremppi_properties.createPropertyList();
};

// Called if the data change
tremppi_properties.modelChanged = function (row_id, colum_id, old_val, new_val, row) {
    var type = this.name.split("_")[0];
    var index = this.name.split("_")[1];
    var column_name = this.columns[colum_id].name;

    // In data, update the row count, if necessary
    if (type === "data") {
        var columns = this.data[row_id].columns;
        // If is not last and is empty
        if (tremppi_common.isEmpty(columns) && row_id !== (this.data.length - 1)) {
            this.remove(row_id);
        } else if (!tremppi_common.isEmpty(columns) && row_id === (this.data.length - 1)) {
            this.append(this.data.length, new Array(this.columns.length), true, true);
        }
    }

    // Store the data
    var row_count = type === "data" ? this.data.length - 1 : this.data.length;
    properties[index][type] = new Array(row_count);
    for (var row_i = 0; row_i < row_count; row_i++) {
        properties[index][type][row_i] = {};
        properties[index][type][row_i].id = this.data[row_i].originalIndex;
        properties[index][type][row_i].values = {};
        for (var column_i = 0; column_i < this.columns.length; column_i++) {
            var column_name = this.columns[column_i].name;
            properties[index][type][row_i].values[column_name] = this.data[row_i].columns[column_i];
        }
    }

    // Remove the property in case it's empty
    if (type === "data" && this.data.length === 1)
        tremppi_properties.remove(index);
    tremppi_common.save("properties");
};

tremppi_properties.createPropertyList = function () {
    // Create the holders
    $("#properties_container").html("");
    grids = [];
    for (var i = 0; i < properties.length; i++) {
        var property_name = 'property_' + i;
        $("#properties_container").append('<div id="' + property_name + '"></div>');
        $("#" + property_name).append('<div id="desc_' + i + '"></div>');
        $("#" + property_name).append('<div id="data_' + i + '"></div><br />');
    }

    // Fill the tables
    for (var i = 0; i < properties.length; i++) {
        grids.push(new EditableGrid("desc_" + i));
        grids[i * 2].load({"metadata": tremppi_properties.metadata["header"], "data": properties[i].desc});
        grids[i * 2].renderGrid("desc_" + i, "property");
        grids[i * 2].modelChanged = tremppi_properties.modelChanged;

        grids.push(new EditableGrid("data_" + i, {enableSort: false}));
        grids[i * 2 + 1].load({"metadata": tremppi_properties.metadata["time_series"], "data": properties[i].data});
        grids[i * 2 + 1].renderGrid("data_" + i, "property");
        grids[i * 2 + 1].modelChanged = tremppi_properties.modelChanged;
        // Add empty row if the last is not empty (only on creation of the new one, must be present due to a EditGraph bug)
        if (!tremppi_common.isEmpty(grids[i * 2 + 1].data[grids[i * 2 + 1].data.length - 1].columns))
            grids[i * 2 + 1].append(grids[i * 2 + 1].data.length, new Array(grids[i * 2 + 1].columns.lenght));
    }
};


