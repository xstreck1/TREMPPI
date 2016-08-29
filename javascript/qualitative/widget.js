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

tremppi.qualitative.getGrid = function (grid_name) {
    return {
        name: grid_name,
        header: grid_name,
        show: {
            header: true
        },
        columns: [
            {field: 'name', caption: 'Label', size: '20%', sortable: true},
            {field: 'entries', caption: '#', size: '10%'},
            {field: 'value', caption: 'Elements', size: '70%'}
        ]
    };
};

tremppi.qualitative.setPanel = function (panel) {
    tremppi.widget[panel] = $('#table_' + panel).w2grid(tremppi.widget.getGrid(panel));
};

tremppi.qualitative.createPanelContent = function (data, source, panel) {
    tremppi.qualitative[panel].values = data.values;
    tremppi.qualitative[panel].records = tremppi.qualitative.valuesToRecords(data.values);
    tremppi.qualitative[panel].header = source;
    tremppi.qualitative[panel].refresh();
    tremppi.report.setDescription(panel, data.setup);
};

tremppi.qualitative.valuesSetter = function (source, panel) {
    return function (data) {
        tremppi.qualitative.createPanelContent(data, source, panel);
        tremppi.log(source + " loaded successfully.");

        // Create the comparison
        if (panel === 'right') {
            var sel_vals = tremppi.qualitative['left'].values;
            var dif_vals = tremppi.qualitative['mid'].values = [];
            var cmp_vals = tremppi.qualitative['right'].values;
            for (var i = 0; i < sel_vals.length; i++) {
                var sel_val = sel_vals[i];
                var cmp_val = tremppi.report.findByName(cmp_vals, sel_val.name);
                if (typeof cmp_val.name !== 'undefined') {
                    var dif_val = {name: sel_val.name, data: []};
                    // Add those in select
                    for (var j = 0; j < sel_val.data.length; j++) {
                        var cmp_data = tremppi.report.findByName(cmp_val.data, sel_val.data[j].name);
                        if (typeof cmp_data.name !== 'undefined') {
                            dif_val.data.push({
                                name: sel_val.data[j].name,
                                portion: (sel_val.data[j].portion - cmp_data.portion)
                            });
                        } else {
                            dif_val.data.push({
                                name: sel_val.data[j].name,
                                portion: sel_val.data[j].portion
                            });
                        }
                    }
                    // Add those in compare only
                    for (var j = 0; j < cmp_val.data.length; j++) {
                        var sel_data = tremppi.report.findByName(sel_val.data, cmp_val.data[j].name);
                        if (typeof sel_data.name === 'undefined') {
                            dif_val.data.push({
                                name: cmp_val.data[j].name,
                                portion: -cmp_val.data[j].portion
                            });
                        }
                    }
                    dif_vals.push(dif_val);
                }
            }
            var mid = {
                "values": dif_vals,
                "setup": {
                    "s_name": tremppi.qualitative.left.header + " - " + tremppi.qualitative.right.header
                }
            };
            tremppi.qualitative.createPanelContent(mid, source, 'mid');
        }
    };
};

tremppi.qualitative.valuesToRecords = function (values) {
    var records = [];
    for (var i = 0; i < values.length; i++) {
        var record = {name: values[i].name, entries: 0, value: ""};
        for (var j = 0; j < values[i].data.length; j++) {
            if (values[i].data[j].portion !== 0.0) {
                record.value += "<b>" + values[i].data[j].name + "</b>:" + (Math.round(values[i].data[j].portion * 10000) / 100) + ", ";
                record.entries++;
            }
        }
        records.push(record);
    }
    return records;
};