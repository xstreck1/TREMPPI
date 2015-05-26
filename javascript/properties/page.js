/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi */

tremppi.properties.page = function () {
    if (typeof tremppi.properties.setup === 'undefined') {
        $("#widget").html("The preperty description has not been configured yet. The Tremppi Spawn command needs to be called.");
    }
    else {
        tremppi.layout.hideToolbar('main');
        $("#widget").append('<div id="property_list"></div>');
        $("#widget").append('<div id="property_detail"></div>');
        tremppi.properties.list = $('#property_list').w2grid(tremppi.properties.makeList());
        tremppi.properties.detail = $('#property_detail').w2grid(tremppi.properties.makeDetail());
    }
};

tremppi.properties.setData = function (data) {
    tremppi.properties.setDefaultData(data);

    if (typeof tremppi.properties.detailed === 'undefined')
        tremppi.properties.detailed = -1;

    tremppi.properties.listControls(tremppi.properties.list);
    tremppi.properties.list.onClick = tremppi.properties.listSelect;

    tremppi.properties.detailControls(tremppi.properties.detail);
    if (tremppi.properties.detailed !== -1) {
        tremppi.properties.listSelect({recid: tremppi.properties.detailed});
    }
};

tremppi.properties.save = function () {
    tremppi.save();
};

tremppi.properties.setDefaultData = function (data) {
    if (typeof data.records === 'undefined')
        data.records = [];
};