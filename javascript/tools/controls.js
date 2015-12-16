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

tremppi.tools.controls = function () {
    tremppi.toolbar.onClick = tremppi.tools.toolbarClick;
};

tremppi.tools.toolbarClick = function (event) {
};

tremppi.tools.killAll = function (command) {
    var url = tremppi.getProjectAddress() + "?killAll";
    $.ajax({
        type: "POST",
        url: url,
        success: function (res) {
            tremppi.log("Kill all executed successfully.");
            tremppi.tools.getCommands();
        },
        fail: function (res) {
            tremppi.log("Kill all failed.", 'error');
        }
    });
};

tremppi.tools.addToQueue = function (command) {
    var url = tremppi.getProjectAddress() + "?tremppi+" + command;
    if (command === 'clean'){
        if (!confirm('Do you really want to call ' + command + '? This operation is irreversible.')) {
            return;
        }
    }
    $.ajax({
        type: "POST",
        url: url,
        success: function (res) {
            tremppi.log(command + " executed successfully.");
            tremppi.tools.getCommands();
        },
        fail: function (res) {
            tremppi.log(command + " failed.", 'error');
        }
    });
};