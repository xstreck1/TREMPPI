/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi */

tremppi.occurence.findByName = function (list, name) {
    for (var i = 0; i < list.length; i++) {
        if (list[i].name === name) {
            return list[i];
        }
    }
    return {};
};

tremppi.occurence.valuesToRecords = function(values) {
    var records = [];
    for (var i = 0; i < values.length; i++) {
        var record = {name: values[i].name, text: ""};
        for (var j = 0; j < values[i].data; j++) {
            record.text += values[i].data[j].name + ":" + values[i].data[j].portion + " ";
        }
        records.push(record);
    }
    return records;
};

tremppi.occurence.valuesSetter = function (source, panel) {
    return function (data) {
        tremppi.occurence[panel].values = data.values;
        tremppi.occurence[panel].records = tremppi.occurence.valuesToRecords(data.values);
        tremppi.occurence[panel].header = source;
        tremppi.occurence[panel].refresh();
        tremppi.log(source + " loaded successfully.");

        var sel_vals = tremppi.occurence['left'].values;
        var dif_vals = tremppi.occurence['mid'].values = [];
        var cmp_vals = tremppi.occurence['right'].values;
        for (var i = 0; i < sel_vals.length; i++) {
            var sel_val = sel_vals[i];
            var cmp_val = tremppi.occurence.findByName(cmp_vals, sel_val.name);
            if (typeof cmp_val.name !== 'undefined') {
                var dif_val = {name: sel_val.name};
                dif_vals.push(dif_val);
            }
        }

        
        tremppi.occurence.mid.records = tremppi.occurence.valuesToRecords(dif_vals);
        tremppi.occurence.mid.header = tremppi.occurence.left.header + " - " + tremppi.occurence.right.header;
        tremppi.occurence.mid.refresh();
    };
};