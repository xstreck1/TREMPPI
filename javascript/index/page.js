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

tremppi.index.page = function () {
    $("#widget").append('<div id="project_title">'+ tremppi.project_name + '</div>');
    $("#widget").append('<div id="text_box"></div>');
    $("#text_box").append('<textarea id="text_field" placeholder="Use this form to provide details about your project."></textarea>');
    $("#widget").append('<div id="version_div">' + "TREMPPI " + tremppi.setup.version + '</div>');
};

tremppi.index.layout = function () {
};

tremppi.index.setData = function (data) {
    $("#text_field").val(data.text);
    
    // Enable controls
    tremppi.index.controls();
};

tremppi.index.toolbarClass = function () {
    return {};
};

tremppi.index.getData = function () {
    var text_field = $("#text_field");
    var text_string = text_field.val();
    // console.log(text_string);
    text_string.replace(/\r?\n/g, '<br />');
    return {text: text_string};
};


tremppi.index.beforeUnload = function() {
    tremppi.save();
};