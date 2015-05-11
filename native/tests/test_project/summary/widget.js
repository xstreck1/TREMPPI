/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi */

tremppi.summary.findByName = function (list, name) {
    for (var i = 0; i < list.length; i++) {
        if (list[i].name === name) {
            return list[i];
        }
    }
    return {};
};

tremppi.summary.valuesSetter = function (source, panel) {
    return function (data) {
        tremppi.summary[panel].records = data.records;
        tremppi.summary[panel].header = source;
        tremppi.summary[panel].refresh();
        tremppi.log(source + " loaded successfully.");

        var sel_recs = tremppi.summary['left'].records;
        var dif_recs = tremppi.summary['mid'].records = [];
        var cmp_recs = tremppi.summary['right'].records;
        for (var i = 0; i < sel_recs.length; i++) {
            var sel_rec = sel_recs[i];
            var cmp_rec = tremppi.summary.findByName(cmp_recs, sel_rec.name);
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

        tremppi.summary.mid.header = tremppi.summary.left.header + " - " + tremppi.summary.right.header;
        tremppi.summary.mid.refresh();
    };
};