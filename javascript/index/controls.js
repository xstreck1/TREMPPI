/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi */

tremppi.index.controls = function () {
    tremppi.toolbar.onClick = tremppi.index.toolbarClick;
};

tremppi.index.toolbarClick = function (event) {
    if (event.target === 'save') {
        tremppi.index.save();
    }
};

tremppi.index.save = function () {
    var text_field = $("#text_field");
    var text_string = text_field.val();
    // console.log(text_string);
    text_string.replace(/\r?\n/g, '<br />');
    tremppi.saveData({text: text_string});
};
