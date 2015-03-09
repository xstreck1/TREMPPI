/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.select.operators = [
    " ", "&#61", "&#8800;", "&#60;", "&#8804;", "&#62;", "&#8805;"
];

tremppi.select.loadTable = function () {
    var grid = tremppi.select.grid = $("#selection_table").w2grid(tremppi.data);
    grid.columnGroups = tremppi.select.columnGroups;
    grid.columns = tremppi.select.columns;

    grid.on('change', tremppi.select.changeFunction);
    grid.refresh();
};


tremppi.select.setColumns = function () {
    var columns = tremppi.select.columns = [];
    var columnGroups = tremppi.select.columnGroups = [];

    var addControlColumn = function (column_data) {
        column_data.size = "20px";
        column_data.resizable = false;
        columns.push(column_data);
    };

    addControlColumn({
        field: "select",
        editable: {
            type: 'check'
        }
    });
    addControlColumn({
        field: "add",
        render: function (record, index, col_index) {
            return '<button class="grid_manip" onclick="tremppi.select.add(' + index + ')">+</button>';
        }
    });
    addControlColumn({
        field: "erase",
        render: function (record, index, col_index) {
            return '<button class="grid_manip" onclick="tremppi.select.erase(' + index + ')">X</button>';
        }
    });
    
    columnGroups.push({caption: 'controls', span: columns.length});

    var addDataGroup = function (prefix, group_name) {
        var type_set = [];
        tremppi.data.columns.forEach(function (column) {
            if (column.field.slice(0, 2) === prefix)
                type_set.push(column);
        });
        type_set.sort();
        type_set.forEach(function (column) {
            addControlColumn({
                field: column.field + '_op',
                editable: {
                    type: 'list',
                    items: tremppi.select.operators
                }
            });
            column.size = "80px";
            column.caption = column.field.slice(2);
            columns.push(column);
        });
        
        
        columnGroups.push({caption: group_name, span: type_set.length * 2});
    };

    addDataGroup("K_", "params");
    addDataGroup("C_", "cost");
    addDataGroup("R_", "robustness");
};