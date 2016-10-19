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
/* global tremppi  */

tremppi.properties.page = function () {
    if ((typeof tremppi.properties.setup === 'undefined') || jQuery.isEmptyObject(tremppi.properties.setup)) {
        $("#widget").html('The property description has not been configured yet. The "TREMPPI enumerate" command needs to be called.');
    }
    else {
        $("#widget").append('<div id="property_list"></div>');
        $("#widget").append('<div id="property_detail"></div>');
        tremppi.properties.list = $('#property_list').w2grid(tremppi.properties.makeList());
        tremppi.properties.detail = $('#property_detail').w2grid(tremppi.properties.makeDetail());
    }
};

tremppi.properties.setData = function (data) {
    tremppi.properties.setDefaultData(data);
    tremppi.properties.list.records = data.records;
    tremppi.properties.list.refresh();
    
    tremppi.properties.list.onClick = tremppi.properties.listClick;
    tremppi.properties.listSelect(parseInt(tremppi.getItem("detailed", -1)));
    
    tremppi.properties.controls();
};

tremppi.properties.getData = function () {
    tremppi.properties.detail.mergeChanges();
    tremppi.properties.list.mergeChanges();
    return {records: tremppi.properties.list.records};
};

tremppi.properties.layout = function () {
    
};

tremppi.properties.setDefaultData = function (data) {
    if (typeof data.records === 'undefined')
        data.records = [];
};

tremppi.properties.toolbarClass = function () {
    return {};
};

tremppi.properties.toolbarClass = function () {
    return {};
};

tremppi.properties.beforeUnload = function() {
    tremppi.save();
};