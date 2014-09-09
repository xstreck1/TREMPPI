tremppi_editor.properties_init = function() { // on dom ready    
    tremppi_editor.metadata.header = [];
    tremppi_editor.metadata["header"].push({name: "id", label: "ID", datatype: "string", editable: true});
    tremppi_editor.metadata["header"].push({name: "type", label: "TYPE", datatype: "string", editable: true});
    tremppi_editor.metadata["header"].push({name: "experiment", label: "EXPERIMENT", datatype: "string", editable: true});
    tremppi_editor.metadata["header"][1].values = {"ts": "Time Series"};

    tremppi_editor.metadata.time_series = [];
    tremppi_editor.metadata["time_series"].push({name: "measurements", label: "MEASUREMENTS", datatype: "string", editable: true});
    
    tremppi_editor.createPropertyList();
};