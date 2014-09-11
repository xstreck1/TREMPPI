var http = require("http");
var fs = require("fs");



http.createServer(function(request, response) {
	fs.readdir(".", function(err, files) {
		response.writeHead(200, {"Content-Type": "text/html"});
		for (var i = 0; i < files.length; i++) {
			response.write(files[i] + "<br/>");
		}
		response.write('<a href="./editor/editor.html">Editor</a>');
		console.log("files loaded");
		response.end();
	});
}).listen(8888);