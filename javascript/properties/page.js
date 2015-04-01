/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.properties.setPage = function () {
    $("#widget").append('<div id="property_list"></div>');
    $("#widget").append('<div id="property_detail"></div>');
};

tremppi.properties.setData = function () {
    tremppi.properties.list = $('#property_list').w2grid(tremppi.data.list);  
    tremppi.properties.listControls(tremppi.properties.list);
    tremppi.properties.list.onChange = tremppi.w2ui.changeFunction(tremppi.data.list.columns, tremppi.data.list.records);
    tremppi.properties.list.onClick = tremppi.properties.listSelect;
    
    tremppi.properties.detail = $('#property_detail').w2grid(tremppi.data.detail);
    tremppi.properties.detailControls(tremppi.properties.detail);
};

tremppi.properties.save = function () {
    tremppi.save();
}

tremppi.properties.setDefaultData = function (data) {
    tremppi.properties.tableList(data.list);
    tremppi.properties.tableDetail(data.detail, data.components);
};