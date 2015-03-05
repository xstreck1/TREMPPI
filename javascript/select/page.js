/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.select.widgetDOM = function () {
    $("#widget").append('<div id="selection_table"></div>');
};

tremppi.select.controlsDOM = function () {
};

tremppi.select.controlsJS = function () {
};

tremppi.select.changeFunction = function (event) {
    var column_name = tremppi.data.columns[event.column].field;
    // List items are object, we want to store the text only
    if (typeof event.value_new === 'object')
        tremppi.data.records[event.index][column_name] = event.value_new.text;
    else
        tremppi.data.records[event.index][column_name] = event.value_new;
    tremppi.save();
};

tremppi.select.widgetJS = function () {
    tremppi.select.loadTable();
};

tremppi.select.layout = function () {
};

tremppi.select.defaultData = {
    "name": "grid",
    "show": {},
    "columns": [],
    "records": []
}