/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.browse.refresh_interval = 1000;

tremppi.browse.setServer = function () {
    tremppi.browse.readFiles();
    setInterval(function () {
        tremppi.browse.readFiles();
    }, tremppi.browse.refresh_interval);
};

tremppi.browse.sourceSetter = function (name) {
    return function () {
        $("#frame").attr("src", name + ".html");
    };
};

tremppi.browse.readFiles = function () {
    $.ajax(
            {url: tremppi.common.getServerAddress() + "?command=files",
                success: function (result) {
                    $("#files").empty();
                    if (!result)
                        return;
                    var res = result.split(",");
                    for (var i = 0; i < res.length; i++) {
                        res[i] = res[i].slice(0, -5);
                        $("#files").append(
                                '<div class="page_content" id="' + res[i] + '">' + res[i] + '</div>'
                                );
                        $("#" + res[i]).click(tremppi.browse.sourceSetter(res[i]));
                    }
                },
                error: function (jqXHR, textStatus, errorThrown) {
                    $("#files").html('ERROR: could not load the files.');
                }
            }
    );
};