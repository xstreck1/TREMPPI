/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

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
