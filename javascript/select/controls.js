/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.select.activateControls = function () {
    tremppi.select.grid.toolbar.add({type: 'check', icon: 'w2ui-icon-check', id: 'select', caption: 'All', checked: false});
    tremppi.select.grid.toolbar.add({type: 'break', id: 'break0'});
    tremppi.select.grid.toolbar.add({type: 'menu', id: 'display', caption: 'Display', items: []});
    tremppi.select.grid.toolbar.add({type: 'break', id: 'break1'});
    tremppi.select.grid.toolbar.add({type: 'button', icon: 'w2ui-icon-plus', id: 'add', caption: 'Add'});
    tremppi.select.grid.toolbar.add({type: 'button', icon: 'w2ui-icon-columns', id: 'duplicate', caption: 'Duplicate'});
    tremppi.select.grid.toolbar.add({type: 'button', icon: 'w2ui-icon-cross', id: 'delete', caption: 'Delete'});
    tremppi.select.grid.onToolbar = tremppi.select.toolbarClick;
};

tremppi.select.toolbarClick = function (event) {
    if (event.type === 'toolbar') {
        if (event.target === 'select') {
            tremppi.w2ui.checkAll(event, tremppi.data.records, tremppi.select.grid);
        }
        else if (event.target.slice(0, 8) === 'display:') {
            var subItem = event.originalEvent.subItem;
            subItem.checked = !subItem.checked;
            subItem.icon = subItem.checked ? 'w2ui-icon-plus' : 'w2ui-icon-cross';
            tremppi.select.setGroups();
        }
        else if (event.target === 'add') {
            tremppi.data.records.push({recid: tremppi.w2ui.getFreeRecID(tremppi.data.records)});
            tremppi.select.grid.records = tremppi.data.records;
        }
        else if (event.target === 'duplicate') {
            tremppi.w2ui.duplicateSelected(tremppi.data.records, tremppi.select.grid);
        }
        else if (event.target === 'delete') {
            tremppi.w2ui.deleteSelected(tremppi.data.records, tremppi.select.grid);
        }
        tremppi.select.grid.refresh();
        tremppi.select.save();
    }
    console.log('Clicked: ' + event.target, event);
};

tremppi.select.addColumnsSelection = function (groups) {
    var menu = tremppi.select.grid.toolbar.get('display');
    groups.forEach(function (group) {
        if (group.hideable) {
            group.text = group.caption;
            group.icon = group.checked ? 'w2ui-icon-plus' : 'w2ui-icon-cross';
            menu.items.push(group);
        }
    });
};