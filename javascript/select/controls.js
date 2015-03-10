/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.select.add = function (entry_no) {
    var ids = tremppi.data.records.map(function (entry) {
        return entry.recid;
    });
    ids.sort();
    for (var id = 0; id < ids.length; id++) {
        if (ids[id] !== id) {
            break;
        }
    }
    var new_entry = $.extend({}, tremppi.data.records[entry_no]);
    new_entry.recid = id;
    tremppi.data.records.splice(entry_no, 0, new_entry);
    tremppi.select.grid.records = tremppi.data.records;
    tremppi.select.grid.refresh();
    tremppi.save();
};

tremppi.select.erase = function (entry_no) {
    if (tremppi.data.records.length <= 1) {
        tremppi.log("It is not allowed to erase all the fields in selection.", "error");
    }
    else {
        tremppi.data.records.splice(entry_no, 1);
        tremppi.select.grid.records = tremppi.data.records;
        tremppi.select.grid.refresh();
        tremppi.save();
    }
};

tremppi.select.changeFunction = function (event) {
    var column_name = tremppi.select.grid_data.columns[event.column].field;
    // List items are object, we want to store the text only
    if (typeof event.value_new === 'object')
        tremppi.data.records[event.index][column_name] = event.value_new.text;
    else
        tremppi.data.records[event.index][column_name] = event.value_new;
    tremppi.save();
};

tremppi.select.toolbarClick = function (event) {
    if (event.type === 'click') {
        if (event.target === 'check_all') {
            var new_val = !event.item.checked;
            tremppi.data.records.forEach(function (record) {
                record.select = new_val;
            });
            tremppi.select.grid.records = tremppi.data.records;
            tremppi.select.grid.refresh();
        }
        if (event.target.slice(0, 8) === 'display:') {
            event.subItem.checked = !event.subItem.checked;
            event.subItem.icon = event.subItem.checked ? 'w2ui-icon-plus' : 'w2ui-icon-cross';
            
            var newColumnGroups = [];
            tremppi.select.grid_data.columnGroups.forEach(function (column_group) {
                if (column_group.caption === event.subItem.id) {
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
    }
    console.log('Target: ' + event.target, event);
};

tremppi.select.addColumnsSelection = function () {
    var menu = tremppi.select.toolbar.get('display');
    tremppi.select.grid_data.columnGroups.forEach(function (column_group) {
        if (column_group.caption !== 'controls')
            menu.items.push({text: column_group.caption, icon: 'w2ui-icon-cross', checked: false});
    });
};