/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.select.operators = [
    ' ', '*', '=', '!=', '<', '<=', '>', '>='
];

tremppi.select.getGridData = function () {
    var grid_data = {};
    $.extend(true, grid_data, tremppi.data);
    var columns = grid_data.columns = [];
    var columnGroups = grid_data.columnGroups = [];

    var controlColumns = [];
    var addControlColumn = function (column_data) {
        column_data.size = '25px';
        column_data.resizable = false;
        columns.push(column_data);
    };

    addControlColumn({
        field: 'select',
        caption: 'S',
        editable: {
            type: 'check',
            icon: 'w2ui-icon-cross'
        }
    });
    addControlColumn({
        field: 'group',
        caption: 'G',
        editable: {
            type: 'int',
            step: 1,
            min: 0,
            max: 9
        }
    });
    controlColumns.push('select', 'group');
    columnGroups.push({caption: 'config', span: controlColumns.length, columns: controlColumns, available: true });

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
                field: column.field + '_cmp',
                editable: {
                    type: 'list',
                    items: tremppi.select.operators
                }
            });
            var new_column = {};
            $.extend(true, new_column, column);
            new_column.caption = column.field.slice(prefix.length, -4);
            new_column.size = 12 + new_column.caption.length * 8 + 'px';
            columns.push(new_column);
            
            new_group.columns.push(column.field + '_cmp');
            new_group.columns.push(column.field);
        });

        new_group.span = new_group.columns.length;
        if (new_group.span > 0)
            columnGroups.push(new_group);
    };

    tremppi.data.regulations.forEach(function(regulation) {
        addDataGroup('K_' + regulation.id + '_', 'K<sub>' + regulation.id + '</sub>' + '(' + regulation.regulators.join(',') + ')');
    });
    addDataGroup('C_', 'cost');
    addDataGroup('R_', 'robustness');
    
    return grid_data;
};