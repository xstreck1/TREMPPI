var tremppi_common = {
    promptWithContent: function(var_name) {
        var content = "var " + var_name + " = " + JSON.stringify(window[var_name], null, '\t') + ";";
        prompt("Press ctrl+C to copy", content);
    }
}