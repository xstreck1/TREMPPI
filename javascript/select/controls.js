/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.select.activateControls = function () {
    tremppi.select.grid.toolbar.add({type: 'menu', id: 'display', caption: 'Display', items: []});
    tremppi.select.grid.toolbar.add({type: 'break', id: 'break0'});
    tremppi.select.grid.toolbar.add({type: 'button', icon: 'w2ui-icon-check', id: 'save', caption: 'Save'});
    tremppi.select.grid.toolbar.add({type: 'button', icon: 'w2ui-icon-plus', id: 'add', caption: 'Add'});
    tremppi.select.grid.toolbar.add({type: 'button', icon: 'w2ui-icon-pencil', id: 'duplicate', caption: 'Duplicate'});
    tremppi.select.grid.toolbar.add({type: 'button', icon: 'w2ui-icon-cross', id: 'delete', caption: 'Delete'});
    tremppi.select.grid.onToolbar = tremppi.select.toolbarClick;
    tremppi.select.grid.onChange = tremppi.select.changeFunction;

};

tremppi.select.freeID = function () {
    var ids = tremppi.data.records.map(function (entry) {
        return entry.recid;
    });
    ids.sort();
    for (var id = 0; id < ids.length; id++) {
        if (ids[id] !== id) {
            return id;
        }
    }
    return id;
};

tremppi.select.toolbarClick = function (event) {
    if (event.type === 'toolbar') {
        if (event.target === 'check_all') {
            var new_val = !event.item.checked;
            tremppi.data.records.forEach(function (record) {
                record.select = new_val;
            });
            tremppi.select.grid.records = tremppi.data.records;
            tremppi.select.grid.refresh();
        }
        if (event.target.slice(0, 8) === 'display:') {
            var subItem = event.originalEvent.subItem;
            subItem.checked = !subItem.checked;
            subItem.icon = subItem.checked ? 'w2ui-icon-plus' : 'w2ui-icon-cross';
            var newColumnGroups = [];
            tremppi.select.columnGroups.forEach(function (column_group) {
                if (column_group.caption === subItem.id) {
                    column_group.columns.forEach(function (column) {
                        tremppi.select.grid.toggleColumn(column);
                    });
                    column_group.available = !column_group.available;
                }
                if (column_group.available)
                    newColumnGroups.push(column_group);
            });
            tremppi.select.grid.columnGroups = newColumnGroups;
            tremppi.select.grid.refresh();
        }
        if (event.target === 'save') {
            tremppi.select.save();
        };
        if (event.target === 'add') {
            tremppi.data.records.push({recid: tremppi.select.freeID()});
            tremppi.select.grid.records = tremppi.data.records;
            tremppi.select.grid.refresh();
        }
        if (event.target === 'duplicate') {
            var new_records = [];
            tremppi.select.grid.getSelection().forEach(function (recid) {
                for (var i = 0; i < tremppi.data.records.length; i++) {
                    if (tremppi.data.records[i].recid === recid) {
                        var new_entry = {};
                        $.extend(new_entry, tremppi.data.records[i]);
                        new_entry.recid = tremppi.select.freeID();
                        new_records.push(new_entry.recid);
                        tremppi.data.records.splice(i, 0, new_entry);
                        break;
                    }
                }
            });
            tremppi.select.grid.records = tremppi.data.records;
            tremppi.select.grid.selectNone();
            new_records.forEach(function(recid){tremppi.select.grid.select(recid)});
            tremppi.select.grid.refresh();
            tremppi.save();
        }
        if (event.target === 'delete') {
            tremppi.select.grid.getSelection().forEach(function (recid) {
                for (var i = 0; i < tremppi.data.records.length; i++) {
                    if (tremppi.data.records[i].recid === recid) {
                        tremppi.data.records.splice(i, 1);
                        break;
                    }
                }
            });
            tremppi.select.grid.records = tremppi.data.records;
            tremppi.select.grid.selectNone();
            tremppi.select.grid.refresh();
            tremppi.save();
        }
    }
    console.log('Clicked: ' + event.target, event);
};

tremppi.select.addColumnsSelection = function (columnGroups) {
    var menu = tremppi.select.grid.toolbar.get('display');
    columnGroups.forEach(function (column_group) {
        if (column_group.caption !== 'controls')
            menu.items.push({text: column_group.caption, icon: 'w2ui-icon-cross', checked: false});
    });
};