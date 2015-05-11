/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.select.columnResize = function (event) {
    event.onComplete = function (event) {
        tremppi.data.columns.forEach(function (column) {
            if (column.field === event.field) {
                column.size = (parseInt(column.size.slice(0,-2)) + parseInt(event.resizeBy)) + "px";
            }
        });
        tremppi.select.save();
    };
};

tremppi.select.setGroups = function () {
    tremppi.select.grid.columnGroups = [];
    tremppi.data.groups.forEach(function (group) {
        if (group.checked) {
            group.columns.forEach(function (column) {
                tremppi.select.grid.hideColumn(column);
            });
        }
    });

    tremppi.data.groups.forEach(function (group) {
        if (!group.checked) {
            tremppi.select.grid.columnGroups.push(group);
        }
    });

};

tremppi.select.setGridData = function (data) {
    var columns = [];
    var groups = data.groups = [];

    // Add the check
    columns.push({
        "field": "select",
        "caption": "",
        "size": "25px",
        "resizable": false,
        "editable": {
            "type": "check"
        }
    });
    columns.push({
        "field": "name",
        "caption": "Name",
        "size": "100px",
        "resizable": true,
        "editable": {
            "type": "text"
        }
    });
    groups.push({caption: '', columns: ['select', 'name'], span: 2, master: false, hideable: false, checked: false});

    // Create a new group and add to columns etc.
    var addDataGroup = function (prefix, group_name) {
        // Order by name
        var type_set = [];
        data.columns.forEach(function (column) {
            if (column.field.slice(0, prefix.length) === prefix)
                type_set.push(column);
        });
        type_set.sort();

        var group = {caption: group_name, columns: [], hideable: true, checked: false};

        type_set.forEach(function (column) {
            var new_column = {};
            $.extend(true, new_column, column);

            if (typeof new_column.caption === 'undefined')
                new_column.caption = column.field.slice(prefix.length);
            if (typeof new_column.size === 'undefined')
                new_column.size = 12 + new_column.caption.length * 8 + 'px';
            if (typeof new_column.resizable === 'undefined')
                new_column.resizable = true;
            if (typeof new_column.editable === 'undefined')
                new_column.editable = {type: 'text'};

            columns.push(new_column);
            group.columns.push(new_column.field);
        });

        group.span = group.columns.length;
        if (group.span > 0)
            groups.push(group);
    };

    // Order by type
    for (var target in tremppi.data.regulations) {
        var regulator_list = (typeof tremppi.data.regulations[target] === 'undefined') ? "" : tremppi.data.regulations[target].join(',');
        addDataGroup('K_' + target + '_', 'K<sub>' + target + '</sub>' + '(' + regulator_list + ')');
    }
    addDataGroup('C_', 'cost');
    addDataGroup('R_', 'robustness');

    data.columns = columns;
};