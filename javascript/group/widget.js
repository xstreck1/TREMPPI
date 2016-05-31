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
    tremppi.group[panel].records = data.records;
    tremppi.group[panel].header = data.setup.s_name;
    tremppi.group[panel].refresh();
    tremppi.report.setDescription(panel, data.setup);
};

tremppi.group.findByVector = function (cmp_recs, sel_rec) {
    for (var i = 0; i < cmp_recs.length; i++) {
        var cmp_rec = cmp_recs[i];
        // Skip if different
        if (Object.keys(cmp_rec).length == Object.keys(sel_rec).length) {

            var found = true;
            for (val in cmp_rec) {
               if (val != "count" && cmp_rec[val] != sel_rec[val]) {
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
}

tremppi.group.valuesSetter = function (source, panel) {
    return function (data) {
        tremppi.group.createPanelContent(data, panel);
        tremppi.log(source + " loaded successfully.");

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
    };
};

tremppi.group.getGrid = function (grid_name) {
    var portion = (100 - 40) / 4;
    return {
        name: grid_name,
        header: grid_name,
        show: {
            header: true
        },
        columns: tremppi.group.setup.columns
    };
};