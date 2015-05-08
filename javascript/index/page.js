/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi */

tremppi.index.page = function () {
    tremppi.index.widget();
};

tremppi.index.layout = function () {};

tremppi.index.setData = function (data) {
    $("#text_field").val(data.text);
    
    tremppi.index.controls();
};