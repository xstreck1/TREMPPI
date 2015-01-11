tremppi.properties.load = function() { // on dom ready    
    tremppi.properties.metadata = {};
    tremppi.properties.metadata.header = [];
    tremppi.properties.metadata["header"].push({name: "Name", label: "NAME", datatype: "string", editable: true});
    tremppi.properties.metadata["header"].push({name: "Type", label: "TYPE", datatype: "string", editable: true});
    tremppi.properties.metadata["header"].push({name: "Experiment", label: "EXPERIMENT", datatype: "string", editable: true});
    tremppi.properties.metadata["header"].push({name: "Verify", label: "VERIFY", datatype: "boolean", editable: true});
    tremppi.properties.metadata["header"][1].values = {"TimeSeries": "Time Series"};

    tremppi.properties.metadata.time_series = [];
    tremppi.properties.metadata["time_series"].push({name: "Measurement", label: "MEASUREMENTS", datatype: "string", editable: true});
    
    tremppi.properties.createPropertyList();
};

tremppi.properties.createPropertyList = function () {
    // Create the holders
    $("#properties_container").html("");
    grids = [];
    for (var i = 0; i < tremppi.data.length; i++) {
        var property_name = 'property_' + i;
        $("#properties_container").append('<div id="' + property_name + '"></div>');
        $("#" + property_name).append('<div id="desc_' + i + '"></div>');
        $("#" + property_name).append('<div id="data_' + i + '"></div><br />');
    }

    // Fill the tables
    for (var i = 0; i < tremppi.data.length; i++) {
        grids.push(new EditableGrid("desc_" + i));
        grids[i * 2].load({"metadata": tremppi.properties.metadata["header"], "data": tremppi.data[i].desc});
        grids[i * 2].renderGrid("desc_" + i, "property");
        grids[i * 2].modelChanged = tremppi.properties.modelChanged;

        grids.push(new EditableGrid("data_" + i, {enableSort: false}));
        grids[i * 2 + 1].load({"metadata": tremppi.properties.metadata["time_series"], "data": tremppi.data[i].data});
        grids[i * 2 + 1].renderGrid("data_" + i, "property");
        grids[i * 2 + 1].modelChanged = tremppi.properties.modelChanged;
        // Add empty row if the last is not empty (only on creation of the new one, must be present due to a EditGraph bug)
        if (!tremppi.common.isEmpty(grids[i * 2 + 1].data[grids[i * 2 + 1].data.length - 1].columns))
            grids[i * 2 + 1].append(grids[i * 2 + 1].data.length, new Array(grids[i * 2 + 1].columns.lenght));
    }
};