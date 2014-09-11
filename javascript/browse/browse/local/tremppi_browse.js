var tremppi_browse = {
    init: function() {
        var fs = require('fs');
        fs.readdir(".", files_callback);
    },
    files_callback: function(err, files) {
        for (var i = 0; i < files.length; i++) {
            $("#file_list").append(files[i] + "\n");
        }
    }
};

