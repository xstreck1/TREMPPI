/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.select.operators = ["&#61", "&#8800;", "&#60;", "&#8801;", "&#62;", "&#8802;"];

tremppi.select.loadTable = function () {
    $("#selection_table").html("");
    $("#selection_table").w2grid(tremppi.data).on('change', tremppi.select.changeFunction);
    var grid = w2ui['grid'];
    for (var i = 0; i < grid.columns.length; i++) {
        if (grid.columns[i].field === "erase") {
            grid.columns[i].render = function (record, index, col_index) {
                return '<button class="erase_btn" onclick="tremppi.select.erase('+index+')">X</button>';
            };
        }

        if (typeof grid.columns[i].editable !== "undefined") {
            if (grid.columns[i].editable.type === "list") {
                grid.columns[i].editable.items = ["&#61", "&#8800;", "&#60;", "&#8801;", "&#62;", "&#8802;"];
                grid.columns[i].render = function (record, index, col_index) {
                    var html = this.getCellValue(index, col_index);
                    return html.text || '';
                };
            }
        }
    }
    
    grid.refresh();
};