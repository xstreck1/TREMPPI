/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.properties.setPage = function () {
    $("#widget").append('<div id="property_list"></div>');
    $("#widget").append('<div id="property_detial"></div>');
};


tremppi.properties.setDefaultData = function (data) {
    data.name = "grid";
    if (typeof data.columns === 'undefined')
        data.columns = [];

    if (typeof data.records === 'undefined' )
        data.records = [];
    
    if (typeof data.regulations === 'undefined')
        data.regulations = [];
};