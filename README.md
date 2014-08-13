TREMPPI
=======

Toolkit for Reverse Engineering of Molecular Pathways through Parameter Identification

NOT WORKING AS OF NOW. For working version use http://esther.fi.muni.cz .

3rd C++ libraries included (see the directory for the specific licence): 

* Gecode: http://www.gecode.org/
* sqlite3: http://www.sqlite.org/ 
* sqlite3pp: https://code.google.com/p/sqlite3pp/ 
* json (JsonCPP): http://sourceforge.net/projects/jsoncpp/ 
* Boost Process: http://www.highscore.de/boost/process0.5/

3rd JS libraries included (open-source)

* jQuery (http://jqueryui.com/)
* jQueryUI (http://jqueryui.com/)
* cytoscapeJS (http://cytoscape.github.io/cytoscape.js/)
* qTip2 (http://qtip2.com/)
* paper.js (http://paperjs.org/)

BUILDING
--------

Requirements:

* CMake 3.0 (or higher)
* GCC 4.9.0 (or higher) or Visual Studio compiler November 2013 (or higher) or Clang 3.4.2 (or higher)
* Boost libraries 1.54.0 (or higher)
Optional:

* Qt5 libraries (or higher) 
* GTest 1.7.0 (or higher) - only for testing

USAGE
=====

Spawn
-----
Spawn creates a database based on a network file. 
The database will have the ".sqlite" extension and it is a Sqlite3 database. The database file is created in the same folder as is the network and has the same name.

The data file format is described in the file "Documentation/database_format.md".

The network file format is described in the file "Documentation/network_format.js".