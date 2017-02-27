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

tremppi.qualitative.page = function () {
    if (typeof tremppi.qualitative.setup === 'undefined') {
        $("#widget").html("No qualitative report has been created yet.");
    } else {
        tremppi.report.createPanels();
        for (var i = 0; i < tremppi.report.panels.length; i++) {
            $('#container_' + tremppi.report.panels[i]).html("");
            $("#container_" + tremppi.report.panels[i]).append('<div id="table_' + tremppi.report.panels[i] + '" class="report_content" ></div>');
            $("#container_" + tremppi.report.panels[i]).append('<div id="desc_' + tremppi.report.panels[i] + '" class="description" ></div>');
        }
        tremppi.report.initialPanel();
    }
};

tremppi.qualitative.setData = function (data) {
};

tremppi.qualitative.getData = function () {
    return {};
};

tremppi.qualitative.layout = function () {
};


tremppi.qualitative.toolbarClass = function () {
    return {};
};

tremppi.qualitative.compareData = function (A, B) {
    return true;
};