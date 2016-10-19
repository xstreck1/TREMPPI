/******************************************************************************
 Created by Adam Streck, 2013-2015, adam.streck@fu-berlin.de
 
 This file is part of the Toolkit for Reverse Engineering of Molecular Pathways
 via Parameter Identification (TREMPPI)
 
 This program is free software: you can redistribute it and/or modify it under
 the terms of the GNU General Public License as published by the Free Software
 Foundation, either version 3 of the License, or (at your option) any later
 version.
 
 This program is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License along with
 this program.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

/* global tremppi */

tremppi.properties.isFreezed = function () {
    return typeof tremppi.properties.setup !== 'undefined'
            && typeof tremppi.properties.setup.freezed !== 'undefined'
            && tremppi.properties.setup.freezed === 1;
};

tremppi.properties.controls = function () {
    tremppi.toolbar.onClick = tremppi.properties.toolbarClick;
    tremppi.properties.listControls();
    tremppi.properties.detailControls();
};

tremppi.properties.toolbarClick = function (event) {
    if (event.target === 'save') {
        tremppi.properties.detail.mergeChanges();
        tremppi.properties.list.mergeChanges();
        tremppi.properties.save();
    }
};

tremppi.properties.listControls = function () {
    var listbar = tremppi.properties.list.toolbar;
    listbar.add({type: 'button', id: 'up', caption: '&#8593;'});
    listbar.add({type: 'button', id: 'down', caption: '&#8595;'});
    listbar.add({type: 'break', id: 'break1'});
    listbar.add({type: 'check', icon: 'w2ui-icon-check', id: 'select', caption: 'All', checked: false});
    listbar.add({type: 'break', id: 'break3'});
    listbar.add({type: 'button', icon: 'w2ui-icon-plus', id: 'add', caption: 'Add'});
    listbar.add({type: 'button', icon: 'w2ui-icon-columns', id: 'duplicate', caption: 'Duplicate'});
    listbar.add({type: 'button', icon: 'w2ui-icon-cross', id: 'delete', caption: 'Delete'});
    listbar.add({type: 'break', id: 'break2'});
    listbar.onClick = tremppi.properties.listbarClick;
};

tremppi.properties.detailControls = function () {
    var detailbar = tremppi.properties.detail.toolbar;
    detailbar.add({type: 'button', id: 'up', caption: '&#8593;'});
    detailbar.add({type: 'button', id: 'down', caption: '&#8595;'});
    detailbar.add({type: 'break', id: 'break1'});
    detailbar.add({type: 'button', icon: 'w2ui-icon-plus', id: 'add', caption: 'Add'});
    detailbar.add({type: 'button', icon: 'w2ui-icon-columns', id: 'duplicate', caption: 'Duplicate'});
    detailbar.add({type: 'button', icon: 'w2ui-icon-cross', id: 'delete', caption: 'Delete'});
    detailbar.onClick = tremppi.properties.detailbarClick;
};

tremppi.properties.emptyProperty = function (recID) {
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

tremppi.properties.listbarClick = function (event) {
    var grid = tremppi.properties.list;
    if (event.target === 'up') {
        tremppi.w2ui.up(grid);
    } else if (event.target === 'down') {
        tremppi.w2ui.down(grid);
    } else if (event.target === 'select') {
        tremppi.w2ui.checkAll(event, grid);
    } else if (event.target === 'add') {
        tremppi.w2ui.add(grid, {name: "unnamed property", records: []});
    } else if (event.target === 'duplicate') {
        tremppi.w2ui.duplicateSelected(grid);
    } else if (event.target === 'delete') {
        tremppi.w2ui.deleteSelected(grid);
    }
    grid.refresh();
};

tremppi.properties.listClick = function (event) {
    tremppi.properties.listSelect(parseInt(event.recid));
};

tremppi.properties.listSelect = function (recid) {
    if (recid === -1) {
        return;
    } else {
        var selected = tremppi.w2ui.findByRecID(tremppi.properties.list.records, recid);
        if (typeof tremppi.properties.detailed !== 'undefined')
        {
            tremppi.properties.detail.mergeChanges();
        }

        if (typeof selected === 'undefined') {
            tremppi.setItem("detailed", -1);
        } else {
            tremppi.properties.detailed = selected.recid;
            tremppi.properties.detail.records = selected.records;
            tremppi.properties.detail.header = selected.name;
            tremppi.properties.detail.refresh();
            tremppi.setItem("detailed", recid);
        }
    }
};

tremppi.properties.assignLetters = function (records) {
    var letter = 65;
    records.forEach(function (record) {
        record.id = String.fromCharCode(letter++);
    });
};

tremppi.properties.detailbarClick = function (event) {
    if (tremppi.properties.detailed !== -1) {
        var grid = tremppi.properties.detail;
        if (event.target === 'up') {
            tremppi.w2ui.up(grid);
        } else if (event.target === 'down') {
            tremppi.w2ui.down(grid);
        } else if (event.target === 'add' && grid.records.length < 26) {
            tremppi.w2ui.add(grid, {});
        } else if (event.target === 'duplicate') {
            tremppi.w2ui.duplicateSelected(grid);
        } else if (event.target === 'delete') {
            tremppi.w2ui.deleteSelected(grid);
        }
        tremppi.properties.assignLetters(grid.records);
        grid.refresh();
    }
};