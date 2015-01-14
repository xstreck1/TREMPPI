/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.report = {
    addSetup : function(setup) {
        values = ["date", "name", "pool_size", "select", "selected", "compare", "compared" ];
        for (var i = 0; i < values.length; i++) {
            $("#analysis_setup").append('<div class="decription"><span class="desc_title">' + 
                    values[i] + 
                    ':</span> <span class="desc_content" id="analysis_date">' + 
                    setup[values[i]] + 
                    '</span></div>');
        }
    }
}; 