tremppi.properties.load = function(data) { // on dom ready    
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