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

tremppi.tools.getCommands = function () {
    var url = tremppi.getProjectAddress() + "tools.html?command=get_commands";
    $.ajax({
        type: "POST",
        url: url,
        success: function (data) {
            $('#progress').html('');
            if (data === "") {
                $('<div>NO COMMAND RUNNING</div>').appendTo($("#progress"));
            } else {
                var commands = data.split(" ");
                var current_command = $('<div><span>' + commands[0] + ': </span><span id="progress_no">00.000</span></div>');
                current_command.appendTo($("#progress"));
                for (var i = 1; i < commands.length; i += 3) {
                    $('<div>' + commands[i] + ' at ' + commands[i + 2] + '</div>').appendTo($("#progress"));
                }
                tremppi.tools.getProgress();
            }
        },
        cache: false,
        error: tremppi.logError
    });
};

tremppi.tools.displayProgress = function (data) {
    var result = parseInt(data);
    if (result !== -1) {
        $("#progress_no").html(data);
        setTimeout(tremppi.tools.getProgress, 1000);
    } else {
        tremppi.tools.getCommands();
        tremppi.tools.getLog();
    };
};

tremppi.tools.getProgress = function () {
    var url = tremppi.getProjectAddress() + "tools.html?command=get_progress";
    $.ajax({
        type: "POST",
        url: url,
        success: tremppi.tools.displayProgress,
        cache: false,
        error: tremppi.logError
    });
};

tremppi.tools.getLog = function () {
    var url = tremppi.getProjectAddress() + "tools.html?command=get_log";
    $.ajax({
        type: "POST",
        url: url,
        success: function (data) {
            var textarea = document.getElementById('log_file');
            textarea.value = data;
            textarea.scrollTop = textarea.scrollHeight;
        },
        cache: false,
        error: tremppi.logError
    });
};