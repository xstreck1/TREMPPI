/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi */

tremppi.select.activateControls = function () {
    var gridbar = tremppi.select.grid.toolbar;
    gridbar.add({type: 'button', id: 'up', caption: '&#8593;'});
    gridbar.add({type: 'button', id: 'down', caption: '&#8595;'});
    gridbar.add({type: 'break', id: 'break0'});
    gridbar.add({type: 'check', icon: 'w2ui-icon-check', id: 'select', caption: 'All', checked: false});
    //gridbar.add({type: 'break', id: 'break1'});
    //gridbar.add({type: 'menu', id: 'display', caption: 'Display', items: []});
    gridbar.add({type: 'break', id: 'break2'});
    gridbar.add({type: 'button', icon: 'w2ui-icon-plus', id: 'add', caption: 'Add'});
    gridbar.add({type: 'button', icon: 'w2ui-icon-columns', id: 'duplicate', caption: 'Duplicate'});
    gridbar.add({type: 'button', icon: 'w2ui-icon-cross', id: 'delete', caption: 'Delete'});
    tremppi.select.grid.onToolbar = tremppi.select.toolbarClick;
};

tremppi.select.toolbarClick = function (event) {
    if (event.type === 'toolbar') {
        var records = tremppi.data.records;
        var grid = tremppi.select.grid;
        
        if (event.target === 'select') {
            tremppi.w2ui.checkAll(event, records, grid);
        }
        else if (event.target === 'up') {
            tremppi.w2ui.up(grid, records);
            grid.records = records;
        }
        else if (event.target === 'down') {
            tremppi.w2ui.down(grid, records);
            grid.records = records;
        }
        else if (event.target === 'display') {
            return;
        }
        else if (event.target.slice(0, 8) === 'display:') {
            var subItem = event.originalEvent.subItem;
            subItem.checked = !subItem.checked;
            subItem.icon = subItem.checked ? 'w2ui-icon-plus' : 'w2ui-icon-cross';
            tremppi.select.setGroups();
        }
        else if (event.target === 'add') {
            var free_recid = tremppi.w2ui.getFreeRecID(records);
            records.push({recid: free_recid, name: "select" + free_recid});
            grid.records = records;
        }
        else if (event.target === 'duplicate') {
            tremppi.w2ui.duplicateSelected(records, grid);
        }
        else if (event.target === 'delete') {
            tremppi.w2ui.deleteSelected(records, grid);
        }
        grid.refresh();
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