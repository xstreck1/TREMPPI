tremppi.properties.setServer = function() {
    $("#control_panel").html("");
    $("#control_panel").append("CREATE " + '<button onclick="tremppi.properties.add()">NEW</button> PROPERTY. (TO DELELETE ONE, ERASE ALL MEASUREMENTS) <br />');
    $("#control_panel").append('<button onclick="tremppi.properties.selectAll(true)">SELECT ALL</button> <button onclick="tremppi.properties.selectAll(false)">DESELECT ALL</button>');
    $("#control_pad").height(($("#control_panel").height() + 4) + "px");
};