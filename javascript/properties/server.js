tremppi.properties.setServer = function() {
    $("#controls").html("");
    $("#controls").append("CREATE A " + '<button onclick="tremppi.properties.add()">NEW</button> PROPERTY. (TO DELELETE ONE, ERASE ALL MEASUREMENTS) <br />');
    $("#controls").append('<button onclick="tremppi.properties.selectAll(true)">SELECT ALL</button> <button onclick="tremppi.properties.selectAll(false)">DESELECT ALL</button>');
};