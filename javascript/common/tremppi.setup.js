/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

var self = tremppi.common;
$(window).ready(
        $.getJSON(self.data_file,
                function (data) {
                    // load data
                    tremppi.data = data;
                    tremppi[self.widget].load();
                    // set a function that will set the server functionality
                    self.addServerContent(tremppi[self.widget].setServer);
                }
        )
        );
