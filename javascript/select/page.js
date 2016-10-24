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

tremppi.select.page = function () {
    if (typeof tremppi.select.setup === 'undefined' || jQuery.isEmptyObject(tremppi.select.setup)) {
        $("#widget").html('The property description has not been configured yet. The "TREMPPI freeze" command needs to be called.');
    }
    else {  
        $("#widget").append('<div id="selection_table"></div>');
            var grid_class = {
            name: 'grid',
            header: 'the list of selections',
            show: {
                toolbar: !tremppi.final, 
                selectColumn: false, 
                toolbarSave: false,
                toolbarReload: false, 
                toolbarSearch: false, 
                toolbarInput: false,
                toolbarColumns: false,
                toolbarAdd: false, 
                toolbarDelete: false
            },
            columns: tremppi.select.setup.columns,
            columnGroups: tremppi.select.setup.groups
        };
        tremppi.select.grid = $('#selection_table').w2grid(grid_class); 
    }
};

tremppi.select.layout = function () {
};

tremppi.select.setData = function (data) {
    tremppi.select.setDefaultData(data);
    tremppi.select.grid.records = data.records;
    tremppi.select.grid.refresh();
        
    // Enable controls
    tremppi.select.addButtons(); 
    tremppi.select.controls();
};


tremppi.select.getData = function () {
    tremppi.select.grid.mergeChanges();
    return {records: tremppi.select.grid.records};
};

tremppi.select.setDefaultData = function (data) {
    if (typeof data.records === 'undefined')
        data.records = [];
};

tremppi.select.toolbarClass = function () {
    return {};
};

tremppi.select.beforeUnload = function() {
    tremppi.save();
};