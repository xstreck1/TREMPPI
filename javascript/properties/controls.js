/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi */

tremppi.properties.listControls = function (list) {
    var listbar = list.toolbar;
    listbar.add({type: 'button', id: 'up', caption: '&#8593;'});
    listbar.add({type: 'button', id: 'down', caption: '&#8595;'});
    listbar.add({type: 'break', id: 'break0'});
    listbar.add({type: 'button', icon: 'w2ui-icon-plus', id: 'add', caption: 'Add'});
    listbar.add({type: 'button', icon: 'w2ui-icon-columns', id: 'duplicate', caption: 'Duplicate'});
    listbar.add({type: 'button', icon: 'w2ui-icon-cross', id: 'delete', caption: 'Delete'});
    listbar.add({type: 'break', id: 'break1'});
    listbar.add({type: 'check', icon: 'w2ui-icon-check', id: 'validate', caption: 'Validate', checked: false});
    listbar.add({type: 'check', icon: 'w2ui-icon-check', id: 'witness', caption: 'Witness', checked: false});
    listbar.add({type: 'check', icon: 'w2ui-icon-check', id: 'robustness', caption: 'Robustness', checked: false});
    listbar.add({type: 'check', icon: 'w2ui-icon-check', id: 'simulate', caption: 'Simulate', checked: false});
    list.onToolbar = tremppi.properties.listClick;
};

tremppi.properties.detailControls = function (detail) {
    var detailbar = detail.toolbar;
    detailbar.add({type: 'button', id: 'up', caption: '&#8593;'});
    detailbar.add({type: 'button', id: 'down', caption: '&#8595;'});
    detailbar.add({type: 'break', id: 'break0'});
    detailbar.add({type: 'button', icon: 'w2ui-icon-plus', id: 'add', caption: 'Add'});
    detailbar.add({type: 'button', icon: 'w2ui-icon-columns', id: 'duplicate', caption: 'Duplicate'});
    detailbar.add({type: 'button', icon: 'w2ui-icon-cross', id: 'delete', caption: 'Delete'});
    detail.onToolbar = tremppi.properties.detailClick;
};

tremppi.properties.emptyProperty = function(recID) {
    var new_prop = {
        recid: recID, 
        type: "series", 
        name: 'property' + recID,
        validate: false,
        witness: false,
        robustness: false,
        simulate: false,
        records: []
    };
    for (var i = 0; i < tremppi.data.components.length; i++) {
        new_prop[tremppi.data.components[i]] = "";
    } 
    return new_prop;
};

tremppi.properties.listClick = function (event) {
    if (event.type === 'toolbar') {
        var records = tremppi.data.list.records;
        var grid = tremppi.properties.list;
        if (event.target === 'up') {
            tremppi.w2ui.up(grid, records);
            grid.records = records;
        }
        else if (event.target === 'down') {
            tremppi.w2ui.down(grid, records);
            grid.records = records;
        }
        else if (event.target === 'validate' || event.target === 'witness' || event.target === 'robustness' || event.target === 'simulate') {
            tremppi.w2ui.checkAll(event, records, grid);
        }
        else if (event.target === 'add') {
            var recID = tremppi.w2ui.getFreeRecID(records);
            records.push(tremppi.properties.emptyProperty(recID));
            grid.records = records;
        }
        else if (event.target === 'duplicate') {
            tremppi.w2ui.duplicateSelected(records, grid);
        }
        else if (event.target === 'delete') {
            tremppi.w2ui.deleteSelected(records, grid);
        }
        grid.refresh();
        tremppi.properties.save();
    }
};

tremppi.properties.listSelect = function (event) {
    tremppi.data.list.records.forEach(function (record) {
        if (record.recid === parseInt(event.recid)) {
            tremppi.properties.detailed = event.recid;
            tremppi.properties.detail.records = record.records;
            tremppi.properties.detail.onChange = tremppi.w2ui.changeFunction(tremppi.properties.detail.columns, record.records);
        }
    });
    tremppi.properties.detail.refresh();
};

tremppi.properties.detailClick = function (event) {
    if (event.type === 'toolbar' && tremppi.properties.detailed !== -1) {
        var records = tremppi.properties.detail.records;
        var grid = tremppi.properties.detail;
        if (event.target === 'up') {
            tremppi.w2ui.up(grid, records);
        }
        else if (event.target === 'down') {
            tremppi.w2ui.down(grid, records);
        }
        else if (event.target === 'add') {
            var recID = tremppi.w2ui.getFreeRecID(records);
            records.push({recid: recID});
            grid.records = records;
        }
        else if (event.target === 'duplicate') {
            tremppi.w2ui.duplicateSelected(records, grid);
        }
        else if (event.target === 'delete') {
            tremppi.w2ui.deleteSelected(records, grid);
        }

        grid.refresh();
        tremppi.properties.save();
    }
};