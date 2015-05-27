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
        $("#widget").append('<div id="property_list"></div>');
        $("#widget").append('<div id="property_detail"></div>');
        tremppi.properties.list = $('#property_list').w2grid(tremppi.properties.makeList());
        tremppi.properties.detail = $('#property_detail').w2grid(tremppi.properties.makeDetail());
    }
};

tremppi.properties.setData = function (data) {
    tremppi.properties.setDefaultData(data);
    tremppi.properties.list.records = data.records;
    tremppi.properties.list.refresh();
    
    tremppi.properties.list.onClick = tremppi.properties.listClick;
    tremppi.properties.listSelect(parseInt(tremppi.getItem("detailed", -1)));
    
    tremppi.properties.controls();
};

tremppi.properties.save = function () {
    tremppi.saveData({records: tremppi.properties.list.records});
};

tremppi.properties.layout = function () {
    
};

tremppi.properties.setDefaultData = function (data) {
    if (typeof data.records === 'undefined')
        data.records = [];
};

tremppi.properties.toolbarClass = function () {
    return {
        name: 'toolbar',
        items: [
            {type: 'button', id: 'save', caption: 'Save', img: 'icon-page', hint: 'Save the data'}
        ]
    };
};