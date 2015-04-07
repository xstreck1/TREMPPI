/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi */

tremppi.properties.tableList = function (list, components) {
    if (typeof list.records === 'undefined') {
        list.records = [];
    }
    list.name = 'list';
    list.multiSelect = false;
    list.columns = [
        {field: 'name', caption: 'Name', size: '200px', resizable: true, sortable: true,
            editable: {type: 'text'}
        },
        {field: 'type', caption: 'Type', size: '60px', resizable: true,
            editable: {type: 'select', items: ["series", "stable", "cycle"]}
        },
        {field: 'validate', caption: 'V', size: '20px', resizable: false,
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
        {field: 'bound', caption: 'Bound', size: '60px', resizable: true,
            editable: {type: 'text'}
        }
    ];

    // Add components contraints
    components.forEach(function (component) {
        list.columns.push(
                {field: component, caption: component, size: '46px',
                    editable: {type: 'text'}
                }
        );
    });

    list.show = {
        toolbar: true, selectColumn: false, toolbarSave: false,
        toolbarReload: false, toolbarSearch: false, toolbarColumns: false,
        toolbarAdd: false, toolbarDelete: false
    };
};

tremppi.properties.tableDetail = function (detail, components) {
    if (typeof detail.records === 'undefined') {
        detail.records = [];
    }
    detail.name = 'detail';
    detail.multiSelect = false;
    detail.show = {
        toolbar: true, selectColumn: false, toolbarSave: false,
        toolbarReload: false, toolbarSearch: false, toolbarColumns: false,
        toolbarAdd: false, toolbarDelete: false
    };

    detail.columns = [];
    detail.columnGroups = [];
    components.forEach(function (component) {
        detail.columnGroups.push({
            span: 2,
            caption: component
        });
        detail.columns.push(
                {field: component + '_value', caption: 'Value', size: '46px',
                    editable: {type: 'text'}
                }
        );
        detail.columns.push(
                {field: component + '_delta', caption: 'Delta', size: '44px',
                    editable: {type: 'select', items: ["", "up", "down", "stay"]}
                }
        );
    });
};