/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.select.setPage = function () {
    $("#widget").append('<div id="selection_table"></div>');
};

tremppi.select.setData = function () {
    var grid_data = tremppi.select.getGridData();
    grid_data.show = {toolbar: true, selectColumn: false, toolbarSave: false, toolbarReload: false, toolbarColumns: false, toolbarAdd: false, toolbarDelete: false};
    tremppi.select.grid = $('#selection_table').w2grid(grid_data);
    tremppi.select.activateControls();
    tremppi.select.addColumnsSelection(grid_data.columnGroups);
};

tremppi.select.save = function () {
    var changes = tremppi.select.grid.getChanges();
    tremppi.select.grid.mergeChanges();
    changes.forEach(function (change) {
        var record = tremppi.w2ui.findByRecid(tremppi.data.records, change.recid);
        for (var val in change) {
            if (val !== 'recid') {
                if (typeof change[val] === 'object') {
                    record[val] = change[val].text;
                }
                else {
                    record[val] = change[val];
                }
            }
        }
    });
    tremppi.save();
};

tremppi.select.setDefaultData = function () {
    tremppi.data.name = "grid";
    if (typeof tremppi.data.columns === 'undefined')
        tremppi.data.columns = [];

    if (typeof tremppi.data.records === 'undefined' || tremppi.data.records.length === 0)
        tremppi.data.records = [{recid: 0}];
};