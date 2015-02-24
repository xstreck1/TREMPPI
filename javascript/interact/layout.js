/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.interact.layout = function() {
    $("#data").height($("body").height() - $("#description").height() -  $("#interact_controls").height());
    $(".graph").height($("#data").height() -  $(".caption").height());
    $("#graph_select").cytoscape('get').resize();
    $("#graph_differ").cytoscape('get').resize();
    $("#graph_compare").cytoscape('get').resize();    
};

