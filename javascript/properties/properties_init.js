tremppi_properties.properties_init = function() { // on dom ready    
    tremppi_properties.metadata.header = [];
    tremppi_properties.metadata["header"].push({name: "Name", label: "NAME", datatype: "string", editable: true});
    tremppi_properties.metadata["header"].push({name: "Type", label: "TYPE", datatype: "string", editable: true});
    tremppi_properties.metadata["header"].push({name: "Experiment", label: "EXPERIMENT", datatype: "string", editable: true});
    tremppi_properties.metadata["header"][1].values = {"TimeSeries": "Time Series"};

    tremppi_properties.metadata.time_series = [];
    tremppi_properties.metadata["time_series"].push({name: "Measurement", label: "MEASUREMENTS", datatype: "string", editable: true});
    
    tremppi_properties.createPropertyList();
};