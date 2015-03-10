/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.select.operators = [
    ' ', '&#61', '&#8800;', '&#60;', '&#8804;', '&#62;', '&#8805;'
];

tremppi.select.loadTable = function () {
    tremppi.select.grid = $('#selection_table').w2grid(tremppi.select.grid_data);
    tremppi.select.grid.on('change', tremppi.select.changeFunction);
};

tremppi.select.setColumns = function () {
    tremppi.select.grid_data = {};
    $.extend(true, tremppi.select.grid_data, tremppi.data);
    var columns = tremppi.select.grid_data.columns = [];
    var columnGroups = tremppi.select.grid_data.columnGroups = [];

    var addControlColumn = function (column_data) {
        column_data.size = '20px';
        column_data.resizable = false;
        columns.push(column_data);
    };

    addControlColumn({
        field: 'select',
        editable: {
            type: 'check',
            icon: 'w2ui-icon-cross'
        }
    });
    addControlColumn({
        field: 'add',
        render: function (record, index, col_index) {
            return '<span class="w2ui-icon-plus" style="font-size: 14px" onclick="tremppi.select.add(' + index + ')"></span>';
        }
    });
    addControlColumn({
        field: 'erase',
        render: function (record, index, col_index) {
            return '<span class="w2ui-icon-cross" style="font-size: 14px" onclick="tremppi.select.erase(' + index + ')"></span>';
        }
    });
    
    columnGroups.push({caption: 'controls', span: columns.length, columns: ['select','add','erase'], available: true });

    var addDataGroup = function (prefix, group_name) {
        var type_set = [];
        tremppi.data.columns.forEach(function (column) {
            if (column.field.slice(0, prefix.length) === prefix)
                type_set.push(column);
        });
        type_set.sort();
        
        var new_group = { caption: group_name, columns: [], available: true };
        
        type_set.forEach(function (column) {
            addControlColumn({
                field: column.field + '_op',
                editable: {
                    type: 'list',
                    items: tremppi.select.operators
                }
            });
            var new_column = {};
            $.extend(true, new_column, column);
            new_column.caption = column.field.slice(prefix.length);
            new_column.size = new_column.caption.length * 10 + 'px';
            columns.push(new_column);
            
            new_group.columns.push(column.field + '_op');
            new_group.columns.push(column.field);
        });

        new_group.span = new_group.columns.length;
        columnGroups.push(new_group);
    };

    tremppi.data.regulations.forEach(function(regulation) {
        addDataGroup('K_' + regulation.id + '_', 'K<sub>' + regulation.id + '</sub>' + '(' + regulation.regulators.join(',') + ')');
    });
    addDataGroup('C_', 'cost');
    addDataGroup('R_', 'robustness');
};