/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

var self = tremppi.common;
tremppi.setup = {
    refresh_interval : 1000
};
// Force fresh load
$.ajaxSetup({
  cache:false
});
//
$(window).ready(
        $.getJSON(self.data_file,
                function (data) {
                    // load data
                    tremppi.data = data;
                    tremppi.setup.addServerContent();
                    tremppi[self.widget].load();
                    // set a function that will set the server functionality
                }
        )
        );

tremppi.setup.layout = function() {
    tremppi.setup.basicLayout();
    tremppi[self.widget].layout();
};

tremppi.setup.setServer = function () {
    tremppi.setup.readFiles();
    setInterval(function () {
        tremppi.setup.readFiles();
    }, tremppi.setup.refresh_interval);
};

tremppi.setup.basicLayout = function() {  
    $("#widget_holder").height($("body").height() - $("#control_panel").height());
};

tremppi.setup.hideServer = function() {
    $("#file_list").html(""); 
    $("#file_list").width(0);
    $("#widget_holder").css("margin-left", 0);
    $("#control_panel").html(""); 
};

// Executes the provided function in the case the server is running
tremppi.setup.addServerContent = function (content_function) {
            $.ajax(
                    {url: tremppi.common.getServerAddress(),
                        success: function (result) {
                            tremppi.common.server_available = true;
                            tremppi.setup.setServer();
                            tremppi[self.widget].setServer();
                            $(window).resize(tremppi.setup.layout);
                            $(window).trigger('resize');
                        },
                        error: function (jqXHR, textStatus, errorThrown) {
                            tremppi.common.server_available = false;
                            console.log("Server not available");
                            tremppi.setup.hideServer();
                            $(window).resize(tremppi.setup.layout);
                            $(window).trigger('resize');
                        }
                    }
            )};
        

tremppi.setup.readFiles = function () {
    $.ajax(
            {url: tremppi.common.getServerAddress() + "?command=files",
                success: function (result) {
                    $("#file_list").empty();
                    if (!result)
                        return;
                    var res = result.split(",");
                    for (var i = 0; i < res.length; i++) {
                        $("#file_list").append('<a class="page_content" id="' + res[i] + '" href="' + res[i] + '">' + res[i].slice(0, -5) + '</a>');
                    }
                }
            }
    );
};

