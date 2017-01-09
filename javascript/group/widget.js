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

tremppi.group.setPanel = function (panel) {
    tremppi.widget[panel] = $('#table_' + panel).w2grid(tremppi.widget.getGrid(panel));
};

tremppi.group.createPanelContent = function (data, panel) {
    tremppi.group[panel].records = tremppi.group.valuesMerger(data.records);
    tremppi.group[panel].header = data.setup.s_name;
    tremppi.group[panel].refresh();
    tremppi.report.setDescription(panel, data.setup);
};

tremppi.group.findByVector = function (cmp_recs, sel_rec) {
    for (var i = 0; i < cmp_recs.length; i++) {
        var cmp_rec = cmp_recs[i];
        // Skip if different
        if (Object.keys(cmp_rec).length === Object.keys(sel_rec).length) {
            var found = true;
            for (var val in cmp_rec) {
                if (val !== "count" && cmp_rec[val] !== sel_rec[val]) { // Should also work with undefined
                    found = false;
                    break;
                }
            }
            if (found) {
                return cmp_rec;
            }
        }
    }
    return null;
};

tremppi.group.findDifferentColumn = function (rec_i, rec_j) {
    for (var i = 0; i < tremppi.group.setup.menu_items.length; i++) {
        if (rec_i[tremppi.group.setup.menu_items[i].id] !== rec_j[tremppi.group.setup.menu_items[i].id]) {
            return i;
        }
    }
    return -1;
};

tremppi.group.valuesMerger = function (records) {
    var new_records = [];
    for (var rec_i = 0; rec_i < records.length; rec_i++) {
        new_records[rec_i] = {};
        for (var col_i = 0; col_i < tremppi.group.setup.menu_items.length; col_i++) {
            if (typeof records[rec_i][tremppi.group.setup.menu_items[col_i].id] === 'undefined' ||
                    tremppi.group.setup.menu_items[col_i].checked === false) {
                // Star the undefined or ignored values
                new_records[rec_i][tremppi.group.setup.menu_items[col_i].id] = '*';
            } else {
                new_records[rec_i][tremppi.group.setup.menu_items[col_i].id]
                        = records[rec_i][tremppi.group.setup.menu_items[col_i].id];
            }
        }
        new_records[rec_i].regulations = records[rec_i].regulations;
        new_records[rec_i].properties = records[rec_i].properties;
        new_records[rec_i].count = records[rec_i].count;
        new_records[rec_i].skip = false;
    }
    var merged = [];
    for (var i = 0; i < new_records.length; i++) {
        if (!new_records[i].skip) {
            for (var j = i + 1; j < new_records.length; j++) {
                var diff = tremppi.group.findDifferentColumn(new_records[i], new_records[j]);
                if (diff === -1) {
                    new_records[i].count += new_records[j].count;
                    new_records[i].properties+= new_records[j].properties;
                    new_records[i].regulations+= new_records[j].regulations;
                    new_records[j].skip = true;
                }
            }
            merged.push(new_records[i]);
        }
    }

    return merged;
};

tremppi.group.valuesSetter = function (source, panel) {
    return function (data) {
        tremppi.group.createPanelContent(data, panel);
        tremppi.log(source + " loaded successfully.");

        if (panel === 'right') {
            var sel_recs = tremppi.group['left'].records;
            var dif_recs = [];
            var cmp_recs = tremppi.group['right'].records;
            for (var i = 0; i < sel_recs.length; i++) {
                var sel_rec = sel_recs[i];
                var cmp_rec = tremppi.group.findByVector(cmp_recs, sel_rec);
                if (cmp_rec !== null) {
                    var dif_rec = {};
                    $.extend(dif_rec, cmp_rec);
                    dif_rec["count"] = sel_rec["count"] - cmp_rec["count"];
                    dif_rec["regulations"] = sel_rec["regulations"] - cmp_rec["regulations"];
                    dif_rec["properties"] = sel_rec["properties"] - cmp_rec["properties"];
                    dif_recs.push(dif_rec);
                }
            }

            var mid = {
                "records": dif_recs,
                "setup": {
                    "s_name": tremppi.group.left.header + " - " + tremppi.group.right.header
                }
            };
            tremppi.group.createPanelContent(mid, 'mid');
        }
    };
};

tremppi.group.getGrid = function (grid_name) {    
    return {
        name: grid_name,
        header: grid_name,
        show: {
            header: true
        },
        columns: tremppi.group.setup.columns
    };
};