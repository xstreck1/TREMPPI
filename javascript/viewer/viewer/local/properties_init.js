$(function() { // on dom ready
    grids = [];
    for (var i = 0; i < properties.length; i++) {
        $("#properties_container").append('<div id="desc_' + i + '"></div>');
        $("#properties_container").append('<div id="data_' + i + '"></div>');
    }

    var metadata = {"header": [], "time_series": []};
    metadata["header"].push({name: "id", label: "ID", datatype: "string", editable: true});
    metadata["header"].push({name: "type", label: "TYPE", datatype: "string", editable: true});
    metadata["header"].push({name: "experiment", label: "EXPERIMENT", datatype: "string", editable: true});
    metadata["header"][1].values = {"ts": "Time Series"};

    metadata["time_series"].push({name: "measurements", label: "MEASUREMENTS", datatype: "string", editable: true});

    var modelChanged = function(rowIndex, columnIndex, oldValue, newValue, row) {
        var type = this.name.split("_")[0];
        var index = this.name.split("_")[1];
        var column_name = this.columns[columnIndex].name;
        properties[index][type][rowIndex].values[column_name] = newValue;
    };

    for (var i = 0; i < properties.length; i++) {
        grids.push(new EditableGrid("desc_" + i));
        grids[i*2].load({"metadata": metadata["header"], "data": properties[i].desc});
        grids[i*2].renderGrid("desc_" + i, "testgrid");
        grids[i*2].modelChanged = modelChanged;
        
        grids.push(new EditableGrid("data_" + i));
        grids[i*2+1].load({"metadata": metadata["time_series"], "data": properties[i].data});
        grids[i*2+1].renderGrid("data_" + i, "testgrid");
        grids[i*2+1].modelChanged = modelChanged;
    }
}); // on dom ready