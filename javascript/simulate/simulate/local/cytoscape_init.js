tremppi_simulate.cytoscape_init = function() { // on dom ready
    var selected_col = '#5555BB';

    $('#graph_object').cytoscape({
        style: cytoscape.stylesheet()
                .selector('node')
                .css({
                    'content': 'data(Name)',
                    'text-valign': 'center',
                    'background-color': 'white',
                    'border-color': 'black',
                    'border-width': 2,
                    'width': 50,
                    'height': 50
                })
                .selector('edge')
                .css({
                    'target-arrow-shape': 'triangle',
                    'content': 'data(score)',
                    'target-arrow-color': 'black',
                    'line-color': 'black',
                    'width': 2.5
                }).selector(':selected')
                .css({
                    'border-color': selected_col,
                    'line-color': selected_col,
                    'target-arrow-color': selected_col
                }),
        layout: {
            name: tremppi_common.hasAllPositions(elements.nodes) ? 'preset' : 'grid',
        },
        elements: elements,
        ready: function() {
            var PADDING = 15;
            this.fit(this.filter('node'), PADDING);
            var my_arr = [$('#graph_object').cytoscape('get')];

            function myFactory(var_i) {
                return function(event) {
                    console.log(my_arr[var_i.val].nodes.lenght);
                };
            }

            for (var i = 0; i < my_arr.length; i++) {
                my_arr[i].on('click', myFactory({"val" :i}));
            }
        }
    });
};