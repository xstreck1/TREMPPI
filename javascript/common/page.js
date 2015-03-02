/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.page = {
    page_list : ["editor", "select", "properties", "function", "interact", "witness"],
    layout : function() {
        $("#widget").height($("body").height() - $("#controls").height() - $("#log_line").height());  
        $("#files").height($("body").height() - $("#log_line").height()); 
    },
    pageDOM : function() {
        this.filesDOM();
        tremppi.widget.controlsDOM();
        tremppi.page.layout();
        tremppi.widget.widgetDOM();
    },
    filesDOM : function() {
        for (var i = 0; i < this.page_list.length; i++) {
            var page = this.page_list[i];
            $("#files").append('<a class="page_content" id="' + page + '_link" href="' + page + '.html">' + page + '</a>');
        }
    }
};


