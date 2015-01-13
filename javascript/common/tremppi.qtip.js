/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.qtip = {
    getConfig: function () {
        this.position = {
            target: 'mouse', // Position it where the click was...
            adjust: {mouse: false} // ...but don't follow the mouse
        };
        this.content = {text: ""};
        this.show = false; // Do not show on mouseover of the graph
    },
    addOnHoverLabeller: function (div_id, elements, labeller) {
        var config = new tremppi.qtip.getConfig();
        var api = $("#" + div_id).qtip(config).qtip('api');

        elements.on("tapdragover", function (eve) {
            var my_data = eve.cyTarget.data();
            api.set("content.text", labeller(my_data));
            api.show();
        });
        elements.on("tapdragout", function () {
            api.hide();
        });
    }
};
