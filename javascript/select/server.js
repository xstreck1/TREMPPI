tremppi.select.val_changed = function () {
    tremppi.select.row_count = $('input[name="row_count"]').val();
};

tremppi.select.setServer = function () {
    $("#control_panel").empty();
    $("#control_panel").append('<button onclick="tremppi.select.set_table()">APPLY</button> selections and show at most ');
    $("#control_panel").append(' <input type="number" name="row_count" min="0" max="999" onchange="tremppi.select.val_changed()" /> rows.');
    $('input[name="row_count"]').val(tremppi.select.row_count).css("font-family", "monospace").css("font-size", "14pt").css("size", "3");
    $("#control_panel").append('<br/>');
    // Add the padding
    $("#control_pad").height($("#control_panel").height() + 10);
};