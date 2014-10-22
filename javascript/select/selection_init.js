tremppi_select.properties_init = function () { // on dom ready    
    tremppi_select.metadata.selections = [];
    tremppi_select.metadata["selections"].push({name: "Name", label: "SELECTION NAME", datatype: "string", editable: true});
    tremppi_select.metadata["selections"].push({name: "Selection", label: "SELECT WHERE", datatype: "string", editable: true});

    tremppi_select.loadSelections();
};