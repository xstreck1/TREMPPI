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
    detailbar.add({type: 'button', icon: 'w2ui-icon-plus', id: 'add', caption: 'Add'});
    detailbar.add({type: 'button', icon: 'w2ui-icon-columns', id: 'duplicate', caption: 'Duplicate'});
    detailbar.add({type: 'button', icon: 'w2ui-icon-cross', id: 'delete', caption: 'Delete'});
    detailbar.add({type: 'break', id: 'break0'});
    detail.onToolbar = tremppi.properties.detailClick;
};

tremppi.properties.listClick = function (event) {
    if (event.type === 'toolbar') {
        if (event.target === 'validate' || event.target === 'witness' || event.target === 'robustness' || event.target === 'simulate') {
            tremppi.w2ui.checkAll(event, tremppi.data.list.records, tremppi.properties.list);
        }
        else if (event.target === 'add') {
            var recID = tremppi.w2ui.getFreeRecID(tremppi.data.list.records);
            tremppi.data.list.records.push({recid: recID, property: 'property ' + recID, records: []});
            tremppi.properties.list.records = tremppi.data.list.records;
            tremppi.properties.list.refresh();
        }
        else if (event.target === 'duplicate') {
            tremppi.w2ui.duplicateSelected(tremppi.data.list.records, tremppi.properties.list);
        }
        else if (event.target === 'delete') {
            tremppi.w2ui.deleteSelected(tremppi.data.list.records, tremppi.properties.list);
        }

        tremppi.properties.list.selectNone();
        tremppi.properties.list.refresh();
        tremppi.properties.save();
    }
};

tremppi.properties.listSelect = function (event) {
    tremppi.data.list.records.forEach(function (record) {
        if (record.recid === parseInt(event.recid)) {
            tremppi.properties.detail.records = record.records;
            tremppi.properties.detail.onChange = tremppi.w2ui.changeFunction(tremppi.properties.detail.columns, record.records);
        }
    });
    tremppi.properties.detail.refresh();
};
