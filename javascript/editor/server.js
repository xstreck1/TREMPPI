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