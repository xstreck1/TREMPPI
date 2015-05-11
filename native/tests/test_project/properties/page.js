/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi */

tremppi.properties.setPage = function () {
    $("#widget").append('<div id="property_list"></div>');
    $("#widget").append('<div id="property_detail"></div>');
};

tremppi.properties.setData = function (data) {
    if (typeof data.components === 'undefined') {
        tremppi.log("tremppi spawn not called / not successful.", 'error');
        return false;
    } else {
        tremppi.properties.list = $('#property_list').w2grid(data.list);
        tremppi.properties.listControls(tremppi.properties.list);
        tremppi.properties.list.onChange = tremppi.w2ui.changeFunction(data.list.columns, data.list.records);
        tremppi.properties.list.onClick = tremppi.properties.listSelect;

        tremppi.properties.detail = $('#property_detail').w2grid(data.detail);
        tremppi.properties.detailControls(tremppi.properties.detail);
        if (tremppi.properties.detailed !== -1) {
            tremppi.properties.listSelect({recid: tremppi.properties.detailed});
        }
        return true;
    }
};

tremppi.properties.save = function () {
    tremppi.save();
};

tremppi.properties.setDefaultData = function (data) {
    if (typeof tremppi.properties.detailed === 'undefined')
        tremppi.properties.detailed = -1;
    if (typeof data.list === 'undefined')
        data.list = {};
    if (typeof data.detail === 'undefined')
        data.detail = {};
    if (typeof data.components === 'undefined') {
        tremppi.log("tremppi spawn not called / not successful.", 'error');
    } else {
        tremppi.properties.tableList(data.list, data.components);
        tremppi.properties.tableDetail(data.detail, data.components);
    }
};