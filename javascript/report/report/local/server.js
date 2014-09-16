/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


tremppi_editor.createServerButtons = function() {
    $("#graph_controls").append('<button id="save_graph">Save</button>');
    $("#save_graph").click(function() {
        tremppi_common.save("report_data");
    });
};