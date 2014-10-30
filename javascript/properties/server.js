tremppi_properties.createControls = function() {
    $("#control_panel").html("");
    $("#control_panel").append("CREATE " + '<button onclick="tremppi_properties.add()">NEW</button> PROPERTY. <br /> (TO DELELETE ONE, ERASE ALL MEASUREMENTS)');
};