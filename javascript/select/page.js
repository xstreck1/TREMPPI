/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.select.setPage = function () {
    $("#widget").append('<div id="selection_table"></div>');
};

tremppi.select.setData = function () {
    tremppi.select.loadTable();
};

tremppi.select.defaultData = {
    "name": "grid",
    "show": {},
    "columns": [{
            "field": "add",
            "type": "add"
        }, {
            "field": "erase",
            "type": "add"
        }],
    "records": []
};