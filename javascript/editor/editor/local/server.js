/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


tremppi_editor.createServerButtons = function() {
    $("#elements_controls").append('<button id="save_elements">Save</button>');
    $("#save_elements").click(function() {
        elements = tremppi_editor.graph.json().elements;
        tremppi_common.save("elements");
    });
    
    $("#properties_controls").append('<button id="save_properties">Save</button>');
    $("#save_properties").click(function() {
       tremppi_common.save("properties");
    });
};