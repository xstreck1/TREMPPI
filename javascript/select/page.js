/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.select.setPage = function () {
    $("#widget").append('<div id="selection_table"></div>');
};

tremppi.select.setData = function () {
    tremppi.select.grid = $('#selection_table').w2grid(tremppi.data);
    tremppi.select.grid.onChange = tremppi.select.changeFunction;
    tremppi.select.setGroups();
    tremppi.select.grid.refresh();
    tremppi.select.activateControls();
    tremppi.select.addColumnsSelection(tremppi.data.groups);
    tremppi.select.grid.onColumnResize = tremppi.select.columnResize;
};

tremppi.select.save = function () {
    tremppi.save();
};

tremppi.select.setDefaultData = function (data) {
    data.name = "grid";
    
    data.show = {toolbar: true, selectColumn: false, toolbarSave: false, 
        toolbarReload: false, toolbarSearch: false, toolbarColumns: false, 
        toolbarAdd: false, toolbarDelete: false};
    
    tremppi.select.setGridData(data);

    if (typeof data.records === 'undefined' )
        data.records = [];
    
    if (typeof data.regulations === 'undefined')
        data.regulations = [];
};