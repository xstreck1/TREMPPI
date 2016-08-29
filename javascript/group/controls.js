/******************************************************************************
 Created by Adam Streck, 2013-2015, adam.streck@fu-berlin.de
 
 This file is part of the Toolkit for Reverse Engineering of Molecular Pathways
 via Parameter Identification (TREMPPI)
 
 This program is free software: you can redistribute it and/or modify it under
 the terms of the GNU General Public License as published by the Free Software
 Foundation, either version 3 of the License, or (at your option) any later
 version.
 
 This program is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License along with
 this program.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

/* global tremppi */

tremppi.group.toolbarClick = function (event) {
    if (event.item.id === 'features' && typeof event.subItem !== 'undefined') {
        console.log(event.subItem.id);
        tremppi.setItem(event.subItem.id, event.subItem.checked);
        if (typeof tremppi.group.setup.menu_items !== 'undefined') {
            for (var i = 0; i < tremppi.group.setup.menu_items.length; i++) {
                if (tremppi.group.setup.menu_items[i].id === event.subItem.id) {
                    tremppi.group.setup.menu_items[i].checked = !tremppi.group.setup.menu_items[i].checked;
                    break;
                }
            }
            if (typeof tremppi.group.left.records !== 'undefined' && tremppi.group.left.records.length !== 0) {
                tremppi.report.pickData(tremppi.group.left.header, 'left');
            }
            if (typeof tremppi.group.right.records !== 'undefined' && tremppi.group.right.records.length !== 0) {
                tremppi.report.pickData(tremppi.group.right.header, 'right');
            }
        }
    }
};