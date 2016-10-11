TREMPPI
=======

Toolkit for Reverse Engineering of Molecular Pathways via Parameter Identification

USER GUIDE: http://dibimath.github.io/TREMPPI/ 

PLEASE DO NOT BUILD FROM THE MASTER BRANCH, IT IS DEVELOPMENT-ONLY! THE STABLE RELEASE CAN BE ACCESSED AT: https://github.com/xstreck1/TREMPPI/releases

Licenced under GNU GPL v3: http://www.gnu.org/licenses/gpl-3.0.en.html

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
Run cmake and then compile with your compiler.

Defines:
* BUILD_TEST: use to build testing binary (default off)
* BOOST_ROOT: use to specify boost location (the folder that holds both libraries and headers)
* GTEST_ROOT: use to specify gtest location, only if building gtest
* GECODE_DIR: use to specify gecode constraint solver binary location, do not mix with GECODE_LOCAL
* GECODE_LOCAL: use to specify gecode constraint solver source location, do not mix with GECODE_DIR 
* TREMPPI_STATIC: use to build with static version of existing libraries (boost, gecode, etc...), will still depend on the runtime libraries and pythonlib

Requirements:

* CMake 2.8 (or higher)
* GCC 4.9.0 (or higher) or Visual Studio 2015 (or higher) or Clang 3.4.2 (or higher)
* Boost libraries 1.54.0 (or higher)
* Gecode 4.2.1 (or higher), get here: http://www.gecode.org/
* Python 3.x.x (or higher), compiled for the same architecture (so 32-bit if you compile 32-bit target and 64-bit if you compile 64-bit target)

Optional:

* GTest 1.7.0 (or higher) - only for testing
