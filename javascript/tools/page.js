/******************************************************************************
Created by Adam Streck, 2013-2015, adam.streck@fu-berlin.de

This file is part of the Toolkit for Reverse Engineering of Molecular Pathways
via Parameter Identification (TREMPPI)

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/
/* global tremppi */

tremppi.tools.tool_list = [
    {
        category: "MODEL",
        tools: [
            {name: "spawn", description: "read a model and create a database of parametrizations based on the model"}
        ]
    },
    {
        category: "STATIC LABELS",
        tools: [
            {name: "bias", description: "label parametrizations with their bias"},
            {name: "express", description: "create the logical expressions for all the parametrizations"},
            {name: "impact", description: "label with the impact of a regulator on its target"},
            {name: "sign", description: "label the edges of individual with their signs"}
        ]
    },
    {
        category: "DYNAMIC LABELS",
        tools: [
            {name: "cost", description: "conduct a model check validate parametrizations agaings selected LTL properties"},
            {name: "trace", description: "conduct a model check and obtain traces for satisfiable properties"},
            {name: "robustness", description: "conduct a model check and obtain robustness for selected properties"}
        ]
    },
    {
        category: "REPORTS",
        tools: [
            {name: "correlations", description: "create a correlations graph report"},
            {name: "qualitative", description: "get qualitative analysis of the known data"},
            {name: "quantitative", description: "get summary of up till now known data"},
            {name: "regulations", description: "create an regulationsion graph based on a statistical analysis"},
            {name: "witness", description: "produce a witness for the given LTL properties (needs valiation first)"}
        ]
    },
    {
        category: "UTILITY",
        tools: [
            {name: "configure", description: "call to explicitly recreate data configuration files"},
            {name: "update", description: "update the javascript and html files in the project directory"},
            {name: "clean", description: "call to remove all the data files"}
        ]
    }
];

tremppi.tools.page = function () {
    tremppi.layout.hideToolbar('main');
    if (!tremppi.final) {
        var commands = $('<div id="commands_holder"></div>').appendTo('#widget');
        for (var i = 0; i < tremppi.tools.tool_list.length; i++) {
            var title = $('<div id="' + tremppi.tools.tool_list[i].category + '_category" class="category">' + tremppi.tools.tool_list[i].category + '</div>').appendTo(commands);
            var tool_set = tremppi.tools.tool_list[i].tools;
            for (var tool_no = 0; tool_no < tool_set.length; tool_no++) {
                var tool_name = tool_set[tool_no].name;
                var button = $('<button id=' + tool_name + ' class="btn">' + tool_name + '</button>').appendTo(commands);
                button.click(function (_tool_name) {
                    return function () {
                        tremppi.tools.addToQueue(_tool_name);
                    };
                }(tool_name));
                $('#' + tool_name).qtip({
                    content: tool_set[tool_no].description
                });
            }
        }
        var reports_div = $('<div id="reports"></div>').appendTo('#widget');
        var progress_list = $('<div id="progress"></div>').appendTo(reports_div);
        var kill_all = $('<button class="btn" id="kill_button" >KILL ALL PROCESSES</button>').appendTo(reports_div);
        kill_all.click(tremppi.tools.killAll);
        var log_file = $('<textarea id="log_file" placeholder="Contents of the log.txt file."></textarea>').appendTo(reports_div);
    }
    else {
        log_file = $('<textarea id="log_file" placeholder="Contents of the log.txt file." style="height: 100%;" ></textarea>').appendTo('#widget');
    }
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

tremppi.tools.getData = function () {
};
