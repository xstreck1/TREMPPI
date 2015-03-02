tremppi.select.val_changed = function () {
    tremppi.select.row_count = $('input[name="row_count"]').val();
};

tremppi.select.setServer = function () {
    tremppi.select.setTable();
    tremppi.select.row_count = 5;
    tremppi.select.addControls();
};

tremppi.select.addControls = function () {
    $("#controls").empty();
    $("#controls").append('<button onclick="tremppi.select.setTable()">APPLY</button> selections and show at most ');
    $("#controls").append(' <input type="number" name="row_count" min="0" max="999" onchange="tremppi.select.val_changed()" /> rows.');
    $('input[name="row_count"]').val(tremppi.select.row_count).css("font-family", "monospace").css("font-size", "14pt").css("size", "3");
    $("#controls").append('<br/>');
    // Add the padding
    $("#control_pad").height($("#controls").height() + 10);
};