tremppi_select.createServerButtons = function() {
    $("#selection_controls").append('<button id="save_selections">Save</button>');
    $("#save_selections").click(function() {
        tremppi_common.save("selections");
    });
};