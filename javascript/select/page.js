/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi */

tremppi.select.page = function () {
    $("#widget").append('<div id="selection_table"></div>');
    if (typeof tremppi.select.configured === 'undefined') {
        $("#widget").html("The selection has not been configured yet. The Tremppi Spawn command needs to be called.");
    }
    else {
        var grid_class = {
            name: 'grid',
            header: 'the list of selections',
            show: {
                toolbar: true, 
                selectColumn: false, 
                toolbarSave: false,
                toolbarReload: false, 
                toolbarSearch: false, 
                toolbarColumns: false,
                toolbarAdd: false, 
                toolbarDelete: false
            },
            columns: tremppi.select.configured.columns,
            columnGroups: tremppi.select.configured.groups
        };
        tremppi.select.grid = $('#selection_table').w2grid(grid_class);
    }
    tremppi.layout.hideToolbar('main');
    
    tremppi.select.widget();
};

tremppi.select.layout = function () {
};

tremppi.select.setData = function (data) {
    tremppi.select.setDefaultData(data);
    tremppi.select.grid.records = data.records;
    tremppi.select.grid.refresh();
    
    tremppi.select.controls();
};

tremppi.select.setDefaultData = function (data) {
    if (typeof data.records === 'undefined')
        data.records = [];
};