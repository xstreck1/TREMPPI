/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi */

tremppi.quantitative.valuesSetter = function (source, panel) {
    return function (data) {
        tremppi.quantitative[panel].records = data.records;
        tremppi.quantitative[panel].header = source;
        tremppi.quantitative[panel].refresh();
        tremppi.log(source + " loaded successfully.");

        var sel_recs = tremppi.quantitative['left'].records;
        var dif_recs = tremppi.quantitative['mid'].records = [];
        var cmp_recs = tremppi.quantitative['right'].records;
        for (var i = 0; i < sel_recs.length; i++) {
            var sel_rec = sel_recs[i];
            var cmp_rec = tremppi.report.findByName(cmp_recs, sel_rec.name);
            if (typeof cmp_rec.name !== 'undefined') {
                var dif_rec = {name: sel_rec.name};
                ['count', 'portion', 'min', 'max', 'mean'].forEach(function (val) {
                    if ((typeof sel_rec[val] !== 'undefined') && (typeof cmp_rec[val] !== 'undefined')) {
                        dif_rec[val] = sel_rec[val] - cmp_rec[val];
                    }
                });
                dif_recs.push(dif_rec);
            }
        }

        tremppi.quantitative.mid.header = tremppi.quantitative.left.header + " - " + tremppi.quantitative.right.header;
        tremppi.quantitative.mid.refresh();
    };
};

tremppi.quantitative.getGrid = function (grid_name) {
    var portion = (100 - 50) / 5;
    return {
        name: grid_name,
        header: grid_name,
        show: {
            header: true
        },
        columns: [
            {field: 'name', caption: 'Name', size: '50%', sortable: true},
            {field: 'count', caption: 'Count', size: portion + '%', sortable: true},
            {field: 'portion', caption: 'Portion', size: portion + '%', sortable: true},
            {field: 'min', caption: 'Min', size: portion + '%', sortable: true},
            {field: 'max', caption: 'Max', size: portion + '%', sortable: true},
            {field: 'mean', caption: 'Mean', size: portion + '%', sortable: true}
        ]
    };
};


tremppi.quantitative.setPanel = function (panel) {
    tremppi.widget[panel] = $('#container_' + panel).w2grid(tremppi.widget.getGrid(panel));
};