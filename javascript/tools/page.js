/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi */

tremppi.tools.tool_list = [
    {name: "configure", description: "call to explicitly recreate data configuration files"},
    {name: "bias", description: "label parametrizations with their bias"},
    {name: "correlations", description: "create a correlations graph report"},
    {name: "express", description: "create the logical expressions for all the parametrizations"},
    {name: "impact", description: "label with the impact of a regulator on its target"},
    {name: "qualitative", description: "get qualitative analysis of the known data"},
    {name: "quantitative", description: "get summary of up till now known data"},
    {name: "regulations", description: "create an regulationsion graph based on a statistical analysis"},
    {name: "sign", description: "label the edges of individual with their signs"},
    {name: "spawn", description: "read a model and create a database of parametrizations based on the model"},
    {name: "update", description: "update the javascript and html files in the project directory"},
    {name: "validate", description: "conduct a model checking to validate parametrizations agaings LTL properties"},
    {name: "witness", description: "produce a witness for the given LTL properties (needs valiation first)"}
];

tremppi.tools.page = function () {
    tremppi.layout.hideToolbar('main');
    var commands_table = $('<table id="commands"></table>').appendTo($('<div id="commands_holder"></div>').appendTo($('#widget')));
    $('<tr><td>EXECUTE TOOL</td><td>DESCRIPTION</td></tr>').appendTo(commands_table);
    for (var i = 0; i < tremppi.tools.tool_list.length; i++) {
        var tool_name = tremppi.tools.tool_list[i].name;
        var row = $('<tr id="' + tool_name + '"></tr>').appendTo(commands_table);
        var button_cell = $('<td></td>').appendTo(row);
        var button = $('<button class="btn">' + tool_name + '</button>').appendTo(button_cell);
        button.click(function(_tool_name) { return function() {tremppi.tools.addToQueue(_tool_name);};}(tool_name));
        var desc_cell = $('<td>'+ tremppi.tools.tool_list[i].description + '</td>').appendTo(row);
    }
    var reports_div = $('<div id="reports"></div>').appendTo($('#widget'));
    var kill_all = $('<button class="btn" id="kill_all" >KILL ALL PROCESSES</button>').appendTo(reports_div);
    kill_all.click(tremppi.tools.killAll);
    var progress_list = $('<div id="progress"></div>').appendTo(reports_div);
    var log_file = $('<textarea id="log_file" placeholder="Contents of the log.txt file."></textarea>').appendTo(reports_div);
};

tremppi.tools.layout = function () {
};

tremppi.tools.setData = function (data) {
    // Enable controls
    tremppi.tools.getCommands();
    tremppi.tools.getLog();
    tremppi.tools.controls();
};

tremppi.tools.toolbarClass = function () {
    return {};
};

tremppi.tools.save = function () {
};
