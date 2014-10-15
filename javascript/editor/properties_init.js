tremppi_editor.properties_init = function() { // on dom ready    
    tremppi_editor.metadata.header = [];
    tremppi_editor.metadata["header"].push({name: "Name", label: "NAME", datatype: "string", editable: true});
    tremppi_editor.metadata["header"].push({name: "Type", label: "TYPE", datatype: "string", editable: true});
    tremppi_editor.metadata["header"].push({name: "Experiment", label: "EXPERIMENT", datatype: "string", editable: true});
    tremppi_editor.metadata["header"][1].values = {"TimeSeries": "Time Series"};

    tremppi_editor.metadata.time_series = [];
    tremppi_editor.metadata["time_series"].push({name: "Measurement", label: "MEASUREMENTS", datatype: "string", editable: true});
    
    tremppi_editor.createPropertyList();
};