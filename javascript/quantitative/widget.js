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

tremppi.quantitative.setPanel = function (panel) {
    tremppi.widget[panel] = $('#table_' + panel).w2grid(tremppi.widget.getGrid(panel));
};

tremppi.quantitative.createPanelContent = function (data, panel) {
    tremppi.quantitative[panel].records = data.records;
    tremppi.quantitative[panel].header = data.setup.s_name;
    tremppi.quantitative[panel].refresh();
    tremppi.report.setDescription(panel, data.setup);
};

tremppi.quantitative.valuesSetter = function (source, panel) {
    return function (data) {
        tremppi.quantitative.createPanelContent(data, panel);
        tremppi.log(source + " loaded successfully.");

        var sel_recs = tremppi.quantitative['left'].records;
        var dif_recs = [];
        var cmp_recs = tremppi.quantitative['right'].records;
        for (var i = 0; i < sel_recs.length; i++) {
            var sel_rec = sel_recs[i];
            var cmp_rec = tremppi.report.findByName(cmp_recs, sel_rec.name);
            if (typeof cmp_rec.name !== 'undefined') {
                var dif_rec = {name: sel_rec.name};
                ['count', 'portion', 'min', 'max', 'mean'].forEach(function (val) {
                    if ((typeof sel_rec[val] !== 'undefined') && (typeof cmp_rec[val] !== 'undefined')) {
                        dif_rec[val] = sel_rec[val] - cmp_rec[val];
                    }
                });
                dif_recs.push(dif_rec);
            }
        }

        var mid = {
            "records": dif_recs,
            "setup": {
                "s_name": tremppi.quantitative.left.header + " - " + tremppi.quantitative.right.header
            }
        };
        tremppi.quantitative.createPanelContent(mid, 'mid');
    };
};

tremppi.quantitative.getGrid = function (grid_name) {
    var portion = (100 - 40) / 4;
    return {
        name: grid_name,
        header: grid_name,
        show: {
            header: true
        },
        columns: [
            {field: 'name', caption: 'Label', size: '40%', sortable: true},
            {field: 'count', caption: 'Count', size: portion + '%', sortable: true},
            {field: 'min', caption: 'Min', size: portion + '%', sortable: true},
            {field: 'max', caption: 'Max', size: portion + '%', sortable: true},
            {field: 'mean', caption: 'Mean', size: portion + '%', sortable: true}
        ]
    };
};