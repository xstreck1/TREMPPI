tremppi.properties.remove = function (index) {
    $("#property_" + index).html("");
    tremppi.data.splice(index, 1);
    tremppi.properties.createPropertyList();
};

// Add new property
tremppi.properties.add = function () {
    tremppi.data.push({
        "desc": [
            {
                "id": 0,
                "values": {
                    "Name": "property_" + tremppi.data.length,
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
                    "Measurement": "tt",
                }
            }
        ]
    });
    tremppi.properties.createPropertyList();
};

// Called if the data change
tremppi.properties.modelChanged = function (row_id, colum_id, old_val, new_val, row) {
    var type = this.name.split("_")[0];
    var index = this.name.split("_")[1];
    var column_name = this.columns[colum_id].name;

    // In data, update the row count, if necessary
    if (type === "data") {
        var columns = this.data[row_id].columns;
        // If is not last and is empty
        if (tremppi.common.isEmpty(columns) && row_id !== (this.data.length - 1)) {
            this.remove(row_id);
        } else if (!tremppi.common.isEmpty(columns) && row_id === (this.data.length - 1)) {
            this.append(this.data.length, new Array(this.columns.length), true, true);
        }
    }

    // Store the data
    var row_count = type === "data" ? this.data.length - 1 : this.data.length;
    tremppi.data[index][type] = new Array(row_count);
    for (var row_i = 0; row_i < row_count; row_i++) {
        tremppi.data[index][type][row_i] = {};
        tremppi.data[index][type][row_i].id = this.data[row_i].originalIndex;
        tremppi.data[index][type][row_i].values = {};
        for (var column_i = 0; column_i < this.columns.length; column_i++) {
            var column_name = this.columns[column_i].name;
            tremppi.data[index][type][row_i].values[column_name] = this.data[row_i].columns[column_i];
        }
    }

    // Remove the property in case it's empty
    if (type === "data" && this.data.length === 1)
        tremppi.properties.remove(index);
    tremppi.common.save();
};

tremppi.properties.selectAll = function (checked) {
    for (var i = 0; i < tremppi.data.length; i++) {
        tremppi.properties.grids[i*2].setValueAt(0, 3, checked, true);
    }
};