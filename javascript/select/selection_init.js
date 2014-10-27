tremppi_select.properties_init = function () { // on dom ready    
    tremppi_select.metadata.selections = [];
    tremppi_select.metadata["selections"].push({name: "Name", label: "NAME", datatype: "string", editable: true});
    tremppi_select.metadata["selections"].push({name: "Selection", label: "WHERE", datatype: "string", editable: true});
    tremppi_select.metadata["selections"].push({name: "Select", label: "SELECT", datatype: "boolean", editable: true });
    tremppi_select.metadata["selections"].push({name: "Compare", label: "COMPARE", datatype: "boolean", editable: true });

    tremppi_select.loadSelections();
};