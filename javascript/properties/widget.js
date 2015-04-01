/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.properties.tableList = function (list) {
    if (typeof list.records === 'undefined') {
        list.records = [];
    }
    list.name = 'list';
    list.columns = [
        {field: 'property', caption: 'Property', size: '200px', resizable: true, sortable: true,
            editable: {type: 'text'}
        },
        {field: 'validate', caption: 'C', size: '20px', resizable: false,
            editable: {type: 'checkbox'}
        },
        {field: 'witness', caption: 'W', size: '20px', resizable: false,
            editable: {type: 'checkbox'}
        },
        {field: 'robustness', caption: 'R', size: '20px', resizable: false,
            editable: {type: 'checkbox'}
        },
        {field: 'simulate', caption: 'S', size: '20px', resizable: false,
            editable: {type: 'checkbox'}
        },
        {field: 'experiement', caption: 'Experiment', size: '100px',
            editable: {type: 'text'}
        }
    ];
    list.show = {
        toolbar: true, selectColumn: false, toolbarSave: false,
        toolbarReload: false, toolbarSearch: false, toolbarColumns: false,
        toolbarAdd: false, toolbarDelete: false
    };
};

tremppi.properties.tableDetail = function (detail) {
    if (typeof detail.records === 'undefined') {
        detail.records = [];
    }
    detail.name = 'detail';
    detail.columns = [
        {field: 'property', caption: 'Property', size: '200px',
            editable: {type: 'text'}
        }
    ];
    detail.show = {
        toolbar: true, selectColumn: false, toolbarSave: false,
        toolbarReload: false, toolbarSearch: false, toolbarColumns: false,
        toolbarAdd: false, toolbarDelete: false
    };
};