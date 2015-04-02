/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.properties.listControls = function (list) {
    var listbar = list.toolbar;
    listbar.add({type: 'button', icon: 'w2ui-icon-plus', id: 'add', caption: 'Add'});
    listbar.add({type: 'button', icon: 'w2ui-icon-columns', id: 'duplicate', caption: 'Duplicate'});
    listbar.add({type: 'button', icon: 'w2ui-icon-cross', id: 'delete', caption: 'Delete'});
    listbar.add({type: 'break', id: 'break0'});
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

tremppi.properties.listClick = function (event) {
    if (event.type === 'toolbar') {
        var records = tremppi.data.list.records;
        var grid = tremppi.properties.list;
        if (event.target === 'validate' || event.target === 'witness' || event.target === 'robustness' || event.target === 'simulate') {
            tremppi.w2ui.checkAll(event, records, grid);
        }
        else if (event.target === 'add') {
            var recID = tremppi.w2ui.getFreeRecID(records);
            records.push({recid: recID, type: "series", name: 'property ' + recID, records: []});
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
            var selection = grid.getSelection();
            if (selection.lenght = 1) {
                var recid = grid.getSelection()[0];
                var i = tremppi.w2ui.iByRecID(records, recid);
                if (i > 0) {
                    var temp = records[i];
                    records[i] = records[i - 1];
                    records[i - 1] = temp;
                }
            }
        }
        if (event.target === 'down') {
            var selection = grid.getSelection();
            if (selection.lenght = 1) {
                var recid = grid.getSelection()[0];
                var i = tremppi.w2ui.iByRecID(records, recid);
                if (i + 1 < records.length) {
                    var temp = records[i];
                    records[i] = records[i + 1];
                    records[i + 1] = temp;
                }
            }
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