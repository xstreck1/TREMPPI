/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi */

tremppi.select.setPage = function () {
    $("#widget").append('<div id="selection_table"></div>');
};

tremppi.select.setData = function (data) {
    if (typeof data.records === 'undefined') {
        tremppi.log("tremppi spawn not called / not successful.", 'error');
        return false;
    }
    else {
        tremppi.select.grid = $('#selection_table').w2grid(data);
        tremppi.select.grid.onChange = tremppi.w2ui.changeFunction(data.columns, data.records);
        tremppi.select.setGroups();
        tremppi.select.grid.refresh();
        tremppi.select.activateControls();
        // tremppi.select.addColumnsSelection(data.groups);
        tremppi.select.grid.onColumnResize = tremppi.select.columnResize;
        return true;
    }
};

tremppi.select.save = function () {
    tremppi.save();
};

tremppi.select.setDefaultData = function (data) {
    data.name = "grid";

    data.show = {toolbar: true, selectColumn: false, toolbarSave: false,
        toolbarReload: false, toolbarSearch: false, toolbarColumns: false,
        toolbarAdd: false, toolbarDelete: false};

    data.multiSelect = false;

    tremppi.select.setGridData(data);

    if (typeof data.records === 'undefined')
        data.records = [];

    if (typeof data.regulations === 'undefined')
        data.regulations = [];
};