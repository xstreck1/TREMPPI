/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.page = {
    page_list: ["editor", "select", "properties", "function", "interact", "witness"],
    layout: function () {
    },
    pageDOM: function () {      
        
    var pstyle = 'border: 0px solid #dfdfdf;';
        $('body').w2layout({
                    name: 'layout',
                    panels: [
                        {type: 'left', style: pstyle, size: 200, content: '<div id="files" ></div>'},
                        {type: 'main', style: pstyle, toolbar: {}, content: '<div id="widget" ></div>'},
                        {type: 'bottom', size: 20, content: '<div id="log_line" ></div>'}
                    ]
                });
        
        this.filesDOM();
        tremppi.widget.controlsDOM();
        tremppi.page.layout();
        tremppi.widget.widgetDOM();
    },
    filesDOM: function () {
        for (var i = 0; i < this.page_list.length; i++) {
            var page = this.page_list[i];
            $("#files").append('<a class="page_content" id="' + page + '_link" href="' + page + '.html">' + page + '</a>');
        }
    }
};


