TREMPPI
=======

Toolkit for Reverse Engineering of Molecular Pathways through Parameter Identification

*FOR USERS: SEE WIKI*

3rd party content
-----------------
C++ libraries included (see the directory for the specific licence): 

* sqlite3: http://www.sqlite.org/ 
* sqlite3pp: https://github.com/iwongu/sqlite3pp/
* json (JsonCPP): http://sourceforge.net/projects/jsoncpp/ 

JS libraries included (open-source):

* jQuery: http://jqueryui.com/
* jQueryUI: http://jqueryui.com/
* cytoscapeJS: http://cytoscape.github.io/cytoscape.js/
* qTip2: http://qtip2.com/
* paper.js: http://paperjs.org/
* EditableGrid: http://www.editablegrid.net/

BUILDING
--------

Requirements:

* CMake 3.0 (or higher)
* GCC 4.9.0 (or higher) or Visual Studio compiler 2015 (or higher) or Clang 3.4.2 (or higher)
* Boost libraries 1.54.0 (or higher)
* Gecode 4.3.0 (or higher), get here: http://www.gecode.org/
* Python 3.x.x (or higher), compiled for the same architecture (so 32-bit if you compile 32-bit target and 64-bit if you compile 64-bit target)

Optional:

* Qt5 libraries (or higher) 
* GTest 1.7.0 (or higher) - only for testing
