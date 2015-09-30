/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi */

tremppi.tools.getCommands = function () {
    var url = tremppi.getServerAddress() + "?getCommands";
    $.ajax({
        type: "GET",
        url: url,
        success: function (data) {
            $('#progress').html('');
            if (data === "") {
                $('<div>NO COMMAND RUNNING</div>').appendTo($("#progress"));
            }
            else {
                var commands = data.split(" ");
                var current_command = $('<div><span>' + commands[0] + ': </span><span id="progress_no">00.000</span></div>');
                current_command.appendTo($("#progress"));
                for (var i = 1; i < commands.length; i += 3) {
                    $('<div>' + commands[i] + ' at ' + commands[i + 2] + '</div>').appendTo($("#progress"));
                }
                tremppi.tools.getProgress();
            }
        }
    });
};

tremppi.tools.getProgress = function () {
    var url = tremppi.getServerAddress() + "?getProgress";
    $.ajax({
        type: "GET",
        url: url,
        success: function (data) {
            var result = parseInt(data);
            if (result !== -1) {
                $("#progress_no").html(data);
                setTimeout(tremppi.tools.getProgress, 1000);
            }
            else {
                tremppi.tools.getCommands();
                tremppi.tools.getLog();
            }
        }
    });
};

tremppi.tools.getLog = function () {
    var url = tremppi.getServerAddress() + "?getLog";
    $.ajax({
        type: "GET",
        url: url,
        success: function (data) {
            var textarea = document.getElementById('log_file');
            textarea.value = data;
            textarea.scrollTop = textarea.scrollHeight;
        }
    });
};