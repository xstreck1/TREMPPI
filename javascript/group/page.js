/******************************************************************************
Created by Adam Streck, 2013-2015, adam.streck@fu-berlin.de

This file is part of the Toolkit for Reverse Engineering of Molecular Pathways
via Parameter Identification (TREMPPI)

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

/* global tremppi */

tremppi.group.page = function () {
    if (typeof tremppi.group.setup === 'undefined') {
        $("#widget").html("No group report has been created yet.");
        tremppi.group.setup = {};
    } else {
        tremppi.report.createPanels();
        for (var i = 0; i < tremppi.report.panels.length; i++) {
            $('#container_' + tremppi.report.panels[i]).html("");
            $("#container_" + tremppi.report.panels[i]).append('<div id="table_' + tremppi.report.panels[i] + '" class="report_content" ></div>');
            $("#container_" + tremppi.report.panels[i]).append('<div id="desc_' + tremppi.report.panels[i] + '" class="description" ></div>');
        }
        tremppi.report.initialPanel();
    }
};

tremppi.group.setData = function (data) {
    tremppi.toolbar.onClick = tremppi.group.toolbarClick;
};

tremppi.group.getData = function () {
    return {};
};

tremppi.group.layout = function () {
};

tremppi.group.toolbarClass = function () {

    var menu_items;
    if (typeof tremppi.group.setup !== 'undefined' && typeof tremppi.group.setup.menu_items !== 'undefined') {
        for (var i = 0; i < tremppi.group.setup.menu_items.length; i++) {
            tremppi.group.setup.menu_items[i].checked =
                    tremppi.getItem(tremppi.group.setup.menu_items[i].id, "true") === "true"; // The bool casting problem solution
        }
        menu_items = tremppi.group.setup.menu_items;
    }
    else {
        menu_items = [];
    }
    var res = {
        name: 'toolbar',
        items: [
            {type: 'menu-check', id: 'features', text: 'Features', items: menu_items }
        ]
    };
    return res;
};