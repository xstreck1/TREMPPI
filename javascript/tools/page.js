/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi */

tremppi.tools.page = function () {
    $("#widget").append('<div id="project_title">'+ tremppi.setup.project_name + '</div>');
    $("#widget").append('<div id="text_box"></div>');
    $("#text_box").append('<textarea id="text_field" placeholder="Use this form to provide details about your project."></textarea>');
};

tremppi.tools.layout = function () {
};

tremppi.tools.setData = function (data) {
    // Enable controls
    tremppi.tools.controls();
};

tremppi.tools.toolbarClass = function () {
    return {
        name: 'toolbar',
        items: [
        ]
    };
};

tremppi.tools.save = function () {
};
