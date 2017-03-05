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
    var url = tremppi.getProjectAddress() + "tools.html?command=kill_all";
    $.ajax({
        type: "POST",
        url: url,
        success: function (res) {
            tremppi.tools.getCommands();
            tremppi.log(res);
        },
        error: tremppi.logError
    });
};

tremppi.tools.addToQueue = function (command) {
    var url = tremppi.getProjectAddress() + "tools.html?command=tremppi&subcommand=" + command;
    if (command === 'erase' || command === 'unfreeze') {
        if (!confirm('Do you really want to call ' + command + '? This operation is irreversible.')) {
            return;
        }
    }
    var success_function = tremppi.tools.displayProgress;
    if (command === 'enumerate' || command === 'erase' || command === 'freeze' || command === 'unfreeze' || command === 'update') {
        success_function = location.reload(true);
    } 
    $.ajax({
        type: "POST",
        url: url,
        success: success_function,
        error: tremppi.logError
    });
};