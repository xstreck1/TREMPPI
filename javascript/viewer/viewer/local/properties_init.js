$(function() { // on dom ready    
    tremppi_viewer.metadata = {"header": [], "time_series": []};
    tremppi_viewer.metadata["header"].push({name: "id", label: "ID", datatype: "string", editable: true});
    tremppi_viewer.metadata["header"].push({name: "type", label: "TYPE", datatype: "string", editable: true});
    tremppi_viewer.metadata["header"].push({name: "experiment", label: "EXPERIMENT", datatype: "string", editable: true});
    tremppi_viewer.metadata["header"][1].values = {"ts": "Time Series"};

    tremppi_viewer.metadata["time_series"].push({name: "measurements", label: "MEASUREMENTS", datatype: "string", editable: true});
    
    tremppi_viewer.createPropertyList();
}); // on dom ready