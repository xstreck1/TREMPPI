tremppi.properties.createControls = function() {
    $("#control_panel").html("");
    $("#control_panel").append("CREATE " + '<button onclick="tremppi.properties.add()">NEW</button> PROPERTY. <br /> (TO DELELETE ONE, ERASE ALL MEASUREMENTS)');
    $("#control_pad").height(($("#control_panel").height() + 4) + "px");
};