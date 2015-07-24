/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi */

tremppi.select.page = function () {
    if (typeof tremppi.select.setup === 'undefined') {
        $("#widget").html("The selection has not been configured yet. The Tremppi Spawn command needs to be called.");
    }
    else {  
        $("#widget").append('<div id="selection_table"></div>');
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
            columns: tremppi.select.setup.columns,
            columnGroups: tremppi.select.setup.groups
        };
        tremppi.select.grid = $('#selection_table').w2grid(grid_class); 
    }
};

tremppi.select.layout = function () {
};

tremppi.select.setData = function (data) {
    tremppi.select.setDefaultData(data);
    tremppi.select.grid.records = data.records;
    tremppi.select.grid.refresh();
        
    // Enable controls
    tremppi.select.addButtons(); 
    tremppi.select.controls();
};


tremppi.select.getData = function () {
    tremppi.select.grid.mergeChanges();
    return {records: tremppi.select.grid.records};
};

tremppi.select.setDefaultData = function (data) {
    if (typeof data.records === 'undefined')
        data.records = [];
};

tremppi.select.toolbarClass = function () {
    return {};
};