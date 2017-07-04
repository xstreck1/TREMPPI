# TREMPPI SBML-mapper

### Description

SBML is a standard file format for easy interchange of computer models of biological processes and systems.
A specification of the format can be found on http://sbml.org .

For qualitative models of gene regulatory networks, where gene activity is modeled as a discrete "activity state"
of network components/nodes or "qualitative species", a specialized SBML package (qualitative SBML) is available.
The package specifications can be found at http://sbml.org/Documents/Specifications/SBML_Level_3/Packages/qual .

This tool allows import and export of qualitative SBML model descriptions to and from the
TREMPPI model database format.

### Functionality Notes

The SBML-importer creates a correct TREMPPI database model for a given qualitative SBML file or throws an appropriate exception
if a parsing error occurrs.

A TREMPPI model (defined by a single row in database table *Parametrizations*) which is exported
with this tool will result in an SBML file which contains the relevant information for the full set of component interactions and
regulatory contexts in the model database, even if some of them are unobservable for the given model. The file satisfies
all requirements for the SBML format, but it may contain unnecessary information for the specific model, to ensure no
information about the model regulatory network is lost.

### Prerequisites

Package **libsbml**. (https://pypi.python.org/pypi/python-libsbml)

Provides functions and an interface for parsing and creating SBML documents in Python.

### Use

All functionality is included in the two Python source files *sbml2db.py* and *db2sbml.py*.

The folder *data* includes example TREMPPI databases and qualSBML model files (see *Examples*).
It is not required for tool functionality.

To run the examples in a Python IDE, uncomment the appropriate lines on the bottom of both source files (see *Examples*).

The tool is meant for implementation into TREMPPI and does not support any direct command line functionality.
To use it, simply pass the appropriate parameters to the two interface functions
*writeSBMLToDBModel()* and *writeDBModelToSBML()*:

##### Import SBML Model File

    Function writeSBMLToDBModel(database_path, sbml_input_path, use_species_name_attribute=False, print_warnings=True)

    Reads a qualSBML document and converts the network model to a TREMPPI database model description.

    Input parameters:
        1) database_path: (String) File path for the TREMPPI model database;
        existing files/databases at this location will be deleted.
        2) sbml_input_path: (String) File path to the qualSBML input file.
        3) use_species_name_attribute: (Logical) If True, the function will try to use the "name" attribute
        of a qualSBML Qualitative Species, instead of the "Id" attribute.
        Will throw an appropriate exception if not successful. (Default setting: False)
        4) print_warnings: (Logical) If True, warnings for non-critical problems will be printed to console.
        (Default setting: True)

    Output:
        If the qualSBML file is successfully parsed, a model database is created at database_path.

##### Export SBML Model File

    Function writeDBModelToSBML(database_path, sbml_output_path, modelRow=1)

    Exports a TREMPPI model to a qualSBML file.

    Input parameters:
        1) database_path (String): File path of the TREMPPI model database.
        2) sbml_output_path (String): File path for the qualSBML output file.
        3) modelRow (Integer): The row of database table "Parametrizations" to be exported as qualSBML.
        Row indexing starts at 1 rather than 0. (Default setting: 1)

    Output:
        If no error occurs, a qualSBML file of the specified model is created at sbml_output_path.


### Examples

There are three example qualitative SBML models included in the project file structure under */data/sbml*.

Two model files were downloaded from Cell Collective (https://cellcollective.org/) and one file was a model
from the GINsim model repository (http://ginsim.org/models_repository) that was exported to the SBML format,
using the GINsim export function. See *References* for download links and related publications.

Additionally there are three SQLite databases under */data/databases*, one of which is a TREMPPI
model database (*bacteriophage.sqlite*) and two others that serve as model databases for imported SBML files
(*modelDB.sqlite* and *modelDB2.sqlite*).

To imort one of the SBML example models to the database *modelDB.sqlite* in a python IDE, just uncomment the
relevant line at the bottom of the Python source file *sbml2db.py* and run the file.
You can also re-import the output SBML file of this tool to the database *modelDB2.sqlite*, by uncommenting and running
the last line, and verify that an export-import loop will result in the same database model description.
To view the databases, you can use a database browser for SQLite (e.g. http://sqlitebrowser.org/).

To export the TREMPPI bacteriophage model to SBML, similarly just uncomment the line at the bottom
of *db2sbml.py*, choose the parameter set (modelRow) of the TREMPPI model database you want to export
and run the file in an IDE. You can alse re-export a previously imported SBML model
by uncommenting the line to export *modelDB.sqlite*. Either way, the resulting SBML file will be created in
*/data/sbml*.

### References (SBML Example models)

    File: data/sbml/CorticalAreaDevelopment_cellCollective_boolean.sbml
    Direct link: https://cellcollective.org/#2035/cortical-area-development
    Type: Boolean network
    Created: 17.03.2013
    Last updated: 22.01.2016
    Publication Reference:
    Giacomantonio CE and Goodhill GJ.; A Boolean model of the gene regulatory network underlying Mammalian
    cortical area development.; 2010 Sep; PLoS Comput Biol; 6 (9): e1000936; pmid:20862356

    MammalianCellCycle_cellCollective_boolean.sbml
    Direct link: https://cellcollective.org/#2396/mammalian-cell-cycle-2006
    Type: Boolean network
    Created: 14.05.2014
    Last updated: 29.03.2016
    Publication Reference:
    Fauré A, Naldi A, Chaouiya C, and Thieffry D; Dynamical analysis of a generic Boolean model for the control
    of the mammalian cell cycle; 2006 Jul; Bioinformatics; 22 (14): e124-e131; pmid:16873462

    NotchPathway_ginsim_multiValued.sbml
    Direct link: http://ginsim.org/node/115
    Type: Multi valued network
    Created: 12.06.2013
    Last updated: Information not available
    Publication Reference:
    Mbodj A, Junion G, Brun C, Furlong EEM, Thieffry D; Logical modelling of Drosophila signalling pathways;
    2013 Sep; Mol Biosyst.; 9(9):2248-58; pmid:23868318

