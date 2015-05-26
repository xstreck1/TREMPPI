/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi */

tremppi.properties.makeList = function () {
    return {
        name: 'list',
        multiSelect: false,
        columns: tremppi.properties.setup.list_columns,
        show : {
            toolbar: true, selectColumn: false, toolbarSave: false,
            toolbarReload: false, toolbarSearch: false, toolbarColumns: false,
            toolbarAdd: false, toolbarDelete: false
        }
    };
};

tremppi.properties.makeDetail = function () {
    return {
        name: 'detail',
        multiSelect: false,
        columns: tremppi.properties.setup.detail_columns,
        columnGroups: tremppi.properties.setup.detail_groups,
        show : {
            toolbar: true, selectColumn: false, toolbarSave: false,
            toolbarReload: false, toolbarSearch: false, toolbarColumns: false,
            toolbarAdd: false, toolbarDelete: false
        }
    };
};