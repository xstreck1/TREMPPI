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
    } else {
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


// Will not work if B has some other properties than A
tremppi.properties.compareData = function (A, B) {
    if (A.records.length !== B.records.length) {
        return false;
    }

    for (var rec_i = 0; rec_i < A.records.length; rec_i++) {
        var rec_A = A.records[rec_i];
        var rec_B = B.records[rec_i];
        var KeysA = JSON.stringify(Object.keys(rec_A).sort());
        var KeysB = JSON.stringify(Object.keys(rec_B).sort());
        if (KeysA !== KeysB) {
            return false;
        }

        for (var rec_prop in rec_A) {
            if (rec_A[rec_prop] === "recods") {
                if (rec_A[rec_prop].length !== rec_B[rec_prop].length) {
                    return false;
                }
                for (var sub_i = 0; sub_i < rec_A[rec_prop].length; sub_i++) {
                    var sub_A = rec_A[rec_prop][sub_i ];
                    var sub_B = rec_A[rec_prop][sub_i ];
                    var KeysA = JSON.stringify(Object.keys(sub_A).sort());
                    var KeysB = JSON.stringify(Object.keys(sub_B).sort());
                    if (KeysA !== KeysB) {
                        return false;
                    }

                    for (var sub_prop in sub_A) {
                        if (sub_A[sub_prop] !== sub_B[sub_prop]) {
                            return false;
                        }
                    }
                }
            } else if (rec_A[rec_prop] !== rec_B[rec_prop]) {
                return false;
            }
        }
    }

    return true;
};