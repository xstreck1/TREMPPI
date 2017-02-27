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

tremppi.properties.makeList = function () {
    if (tremppi.isFinal() || !tremppi.hasControls() || tremppi.properties.isFreezed()) {
        for (var i = 0; i < tremppi.properties.setup.list_columns.length; i++) {
            if (tremppi.properties.setup.list_columns[i].field !== "select") {
                tremppi.properties.setup.list_columns[i].editable = false;
            }
        }
    }

    return {
        name: 'list',
        columns: tremppi.properties.setup.list_columns,
        show: {
            toolbar: !tremppi.isFinal() && tremppi.hasControls() && !tremppi.properties.isFreezed(), selectColumn: false, toolbarSave: false,
            toolbarReload: false, toolbarSearch: false, toolbarColumns: false, toolbarInput: false,
            toolbarAdd: false, toolbarDelete: false
        }
    };
};

tremppi.properties.makeDetail = function () {
    if (tremppi.isFinal() || !tremppi.hasControls() || tremppi.properties.isFreezed()) {
        for (var i = 0; i < tremppi.properties.setup.detail_columns.length; i++) {
            tremppi.properties.setup.detail_columns[i].editable = false;
        }
    }

    return {
        name: 'detail',
        header: 'measurements',
        columns: tremppi.properties.setup.detail_columns,
        columnGroups: tremppi.properties.setup.detail_groups,
        show: {
            toolbar: !tremppi.isFinal() && tremppi.hasControls() && !tremppi.properties.isFreezed(), header: true, selectColumn: false, toolbarSave: false,
            toolbarReload: false, toolbarSearch: false, toolbarColumns: false, toolbarInput: false,
            toolbarAdd: false, toolbarDelete: false
        }
    };
};