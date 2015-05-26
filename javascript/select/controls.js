/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi */


tremppi.select.save = function () {
    tremppi.select.grid.mergeChanges();
    tremppi.saveData({records: tremppi.select.grid.records});
};

tremppi.select.controls = function () {
    var gridbar = tremppi.select.grid.toolbar;
    gridbar.add({type: 'button', id: 'save', caption: 'Save', img: 'icon-page', hint: 'Save the data'});
    gridbar.add({type: 'break', id: 'break0'});
    gridbar.add({type: 'button', id: 'up', caption: '&#8593;'});
    gridbar.add({type: 'button', id: 'down', caption: '&#8595;'});
    gridbar.add({type: 'break', id: 'break1'});
    gridbar.add({type: 'check', icon: 'w2ui-icon-check', id: 'select', caption: 'All', checked: false});
    //gridbar.add({type: 'break', id: 'break2'});
    //gridbar.add({type: 'menu', id: 'display', caption: 'Display', items: []});
    gridbar.add({type: 'break', id: 'break3'});
    gridbar.add({type: 'button', icon: 'w2ui-icon-plus', id: 'add', caption: 'Add'});
    gridbar.add({type: 'button', icon: 'w2ui-icon-columns', id: 'duplicate', caption: 'Duplicate'});
    gridbar.add({type: 'button', icon: 'w2ui-icon-cross', id: 'delete', caption: 'Delete'});
    gridbar.onClick = tremppi.select.toolbarClick;
};

tremppi.select.toolbarClick = function (event) {
    var grid = tremppi.select.grid;
    if (event.target === 'save') {
        tremppi.select.save();
    }
    if (event.target === 'select') {
        tremppi.w2ui.checkAll(event, grid);
    }
    else if (event.target === 'up') {
        tremppi.w2ui.up(grid);
    }
    else if (event.target === 'down') {
        tremppi.w2ui.down(grid);
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
        tremppi.w2ui.add(grid, {name: "unnamed selection"});
    }
    else if (event.target === 'duplicate') {
        tremppi.w2ui.duplicateSelected(grid);
    }
    else if (event.target === 'delete') {
        tremppi.w2ui.deleteSelected(grid);
    }
    grid.refresh();
};