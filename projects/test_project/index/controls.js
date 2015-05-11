/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi */

tremppi.index.controls = function() {
    var text_field = $("#text_field");
    text_field.on('change paste keyup', function(){
         //value
        var text_string = text_field.val();
        console.log(text_string);
        text_string.replace(/\r?\n/g, '<br />');
        tremppi.saveData({text: text_string});
    });
};