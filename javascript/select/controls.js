/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.select.erase = function(entry_no) {
    tremppi.data.records.splice(entry_no, 1);
    w2ui['grid'].records = tremppi.data.records;
    w2ui['grid'].refresh();    
    tremppi.save();
};
