/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.overview.toolbarClick = function (event) {
    if (event.type === 'click') {
        if (event.target === 'all') {
            $('#container_left').css('width', '33.33%').css('display', 'block');
            $('#container_mid').css('width', '33.33%').css('display', 'block');
            $('#container_right').css('width', '33.33%').css('display', 'block');
            tremppi.overview.left.resize();
            tremppi.overview.mid.resize();
            tremppi.overview.right.resize();
        }
        else if (event.target === 'left') {
            $('#container_left').css('width', '100%').css('display', 'block');
            $('#container_mid').css('display', 'none');
            $('#container_right').css('display', 'none');            
            tremppi.overview.left.resize();
        }
        else if (event.target === 'mid') {
            $('#container_left').css('display', 'none');
            $('#container_mid').css('width', '100%').css('display', 'block');
            $('#container_right').css('display', 'none');
            tremppi.overview.mid.resize();
        }
        else if (event.target === 'right') {
            $('#container_left').css('display', 'none');
            $('#container_mid').css('display', 'none');
            $('#container_right').css('width', '100%').css('display', 'block');
            tremppi.overview.right.resize();
        }
    }
    console.log('Clicked: ' + event.target, event);
};