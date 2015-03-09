/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.select.operators = [
    "&#61", "&#8800;", "&#60;", "&#8801;", "&#62;", "&#8802;"
];

tremppi.select.loadTable = function () {
    var grid = tremppi.select.grid = $("#selection_table").w2grid(tremppi.data);
    for (var i = 0; i < grid.columns.length; i++) {
        tremppi.select.columnControls(grid.columns[i]);
    }
    grid.on('change', tremppi.select.changeFunction);
    grid.refresh();
};
