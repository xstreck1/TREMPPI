tremppi.select.load = function () { // on dom ready       
    tremppi.select.metadata = {};
    tremppi.select.metadata.selections = [];
    tremppi.select.metadata["selections"].push({name: "Selection", label: "WHERE", datatype: "string", editable: true});
    tremppi.select.metadata["selections"].push({name: "Select", label: "SELECT", datatype: "boolean", editable: true });
    tremppi.select.metadata["selections"].push({name: "Compare", label: "COMPARE", datatype: "boolean", editable: true });

    tremppi.select.loadSelections();
};

tremppi.select.loadSelections = function () {
    $("#selection_holder").empty();
    var selections_grid = new EditableGrid("selections");
    
    if (tremppi.data.length === 0) {
        tremppi.data = [{
            id: 0,
            values: {
                Name: "",
                Selection: "",
                Select: false,
                Compare: false
            }
        }];
    }

    selections_grid.load({"metadata": tremppi.select.metadata["selections"], "data": tremppi.data});
    selections_grid.renderGrid("selection_holder", "selections");
    selections_grid.modelChanged = tremppi.select.selectionChanged;

    if (selections_grid.data.length === 0 || !tremppi.common.isEmpty(selections_grid.data[selections_grid.data.length - 1].columns))
        selections_grid.append(selections_grid.data.length, {
            values: {
                Name: "",
                Selection: "",
                Select: false,
                Compare: false
            }
        });
};