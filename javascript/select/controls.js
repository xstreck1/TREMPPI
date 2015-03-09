/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.select.add = function (entry_no) {
    var ids = tremppi.data.records.map(function (entry) {
        return entry.recid;
    });
    ids.sort();
    for (var id = 0; id < ids.length; id++) {
        if (ids[id] !== id) {
            break;
        }
    }
    var new_entry = $.extend({}, tremppi.data.records[entry_no]);
    new_entry.recid = id;
    tremppi.data.records.splice(entry_no, 0, new_entry);
    w2ui['grid'].records = tremppi.data.records;
    w2ui['grid'].refresh();
    tremppi.save();
};
tremppi.select.erase = function (entry_no) {
    if (tremppi.data.records.length <= 1) {
        tremppi.log("It is not allowed to erase all the fields in selection.", "error");
    }
    else {
        tremppi.data.records.splice(entry_no, 1);
        w2ui['grid'].records = tremppi.data.records;
        w2ui['grid'].refresh();
        tremppi.save();
    }
};
tremppi.select.changeFunction = function (event) {
    var column_name = tremppi.select.columns[event.column].field;
    // List items are object, we want to store the text only
    if (typeof event.value_new === 'object')
        tremppi.data.records[event.index][column_name] = event.value_new.text;
    else
        tremppi.data.records[event.index][column_name] = event.value_new;
    tremppi.save();
};