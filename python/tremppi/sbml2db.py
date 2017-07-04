#
import os.path
import sqlite3
import libsbml
import re
import itertools


class ParseASTError(Exception):
    def __init__(self, value):
        self.value = value
    def __str__(self):
        return repr(self.value)

'''
Function computeThresholds
    Recursive function that finds the 'leaf' subtries (binary relations: species ~ Integer) of a libsbml.FunctionTerm mathML ASTNode tree
    and computes TREMPPI regulation thresholds from these relations.
Input parameters:
    1) math_AST: Root ASTNode of a libsbml.Transition.FunctionTerm.Math element.
    2) source_threshold_dict: Python dictionary of transition source species with elements: { source name(string):[threshold1(int), threshold2(int), ...] }.
    3) species_Id_maxActivity_dict: Python dictionary of species Ids with corresponding maximum activity values: { species Id(string):maxActivity(int) }.
Output:
    source_threshold_dict is updated with all thresholds defined by this libsbml.FunctionTerm.
'''
def computeThresholds(math_AST, source_threshold_dict, species_Id_maxActivity_dict):
    # if the ASTNode is logical, call computeThresholds for all children ASTNodes
    if math_AST.getType() in [libsbml.AST_LOGICAL_AND, libsbml.AST_LOGICAL_OR, libsbml.AST_LOGICAL_NOT, libsbml.AST_LOGICAL_XOR]:
        for i in range(0, math_AST.getNumChildren()):
            # recursive call
            computeThresholds(math_AST.getChild(i), source_threshold_dict, species_Id_maxActivity_dict)
    # else if the ASTNode is relational, get both child ASTNodes
    elif math_AST.getType() in [libsbml.AST_RELATIONAL_EQ, libsbml.AST_RELATIONAL_GEQ, libsbml.AST_RELATIONAL_GT, libsbml.AST_RELATIONAL_LEQ, libsbml.AST_RELATIONAL_LT, libsbml.AST_RELATIONAL_NEQ]:
        left = math_AST.getLeftChild()
        right = math_AST.getRightChild()
        flipped = False
        # flip nodes if they are ordered ( Integer (relation) source Id )
        if left.getType() == libsbml.AST_INTEGER and right.getType() == libsbml.AST_NAME:
            tmp = left
            left = right
            right = tmp
            flipped = True
        # if ASTNodes are now correctly ordered ( source Id (relation) Integer ), extract the source Id string and int value and update source_threshold_dict with thresholds defined by this relation
        # (only if thresholds are not already in source_threshold_dict for this source and if they are different from 0 and not larger than the sources max. activity)
        if left.getType() == libsbml.AST_NAME and right.getType() == libsbml.AST_INTEGER:
            source = left.getName()
            int_value = right.getInteger()
            # if the relation is '=' or '!=', add thresholds for int_value and int_value + 1 to this transition source
            if math_AST.getType() == libsbml.AST_RELATIONAL_EQ or math_AST.getType() == libsbml.AST_RELATIONAL_NEQ:
                new_thresholds = [x for x in [int_value, int_value + 1] if x not in source_threshold_dict[source] and x != 0 and x <= species_Id_maxActivity_dict[source]]
                source_threshold_dict[source].extend(new_thresholds)
            # else if the relation is 'source >= int value' , add int_value
            elif (math_AST.getType() == libsbml.AST_RELATIONAL_GEQ and not flipped) or (math_AST.getType() == libsbml.AST_RELATIONAL_LEQ and flipped):
                if int_value not in source_threshold_dict[source] and int_value != 0:
                    source_threshold_dict[source].append(int_value)
            # else if the relation is 'source <= int value' , add int_value + 1
            elif (math_AST.getType() == libsbml.AST_RELATIONAL_LEQ and not flipped) or (math_AST.getType() == libsbml.AST_RELATIONAL_GEQ and flipped):
                if int_value + 1 not in source_threshold_dict[source] and int_value + 1 <= species_Id_maxActivity_dict[source]:
                    source_threshold_dict[source].append(int_value + 1)
            # in the same way, add appropriate thresholds for '<' and '>'
            elif (math_AST.getType() == libsbml.AST_RELATIONAL_GT and not flipped) or (math_AST.getType() == libsbml.AST_RELATIONAL_LT and flipped):
                if int_value + 1 not in source_threshold_dict[source] and int_value + 1 <= species_Id_maxActivity_dict[source]:
                    source_threshold_dict[source].append(int_value + 1)
            elif (math_AST.getType() == libsbml.AST_RELATIONAL_LT and not flipped) or (math_AST.getType() == libsbml.AST_RELATIONAL_GT and flipped):
                if int_value not in source_threshold_dict[source] and int_value != 0:
                    source_threshold_dict[source].append(int_value)
        # If 'leaf subtrees' are not ( Integer (relation) source Id ) or ( source Id (relation) Integer ), throw an exception
        else:
            raise ParseASTError("[Error] computeThresholds: Unexpected mathML expression in FunctionTerm: Lowest level expressions must be species~integer comparisons.\nComponent species may only be compared to integer values, and only by the following relations: =, /=, >=, <=, >, < \n")
    # if any non-leaf ASTNode is not logical or relational, throw an exception
    else:
        raise ParseASTError("[Error] computeThresholds: Unexpected mathML expression in FunctionTerm: Only logical and relational mathML operators are allowed. \n")

#end computeThresholds

'''
Function evaluateContext
    Recursive function that evaluates FunctionTerm math elements for a given TREMPPI regulatory context.
Input parameters:
    1) source_ts_dict: Python dictionary of all source components and their threshold states for a regulatory context.
        e.g. context: K_CompC_01 with regulators CompA and CompB -> source_ts_dict = {'CompA':0, 'CompB':1}
    2) math_AST: Root ASTNode of a libsbml.Transition.FunctionTerm.Math element.
Output:
    True, if the regulator threshold states fulfill this FunctionTerm math element and False otherwise.
'''
def evaluateContext(source_ts_dict, math_AST):
    # mathML's logical functions are not required to be binary, therefore we need to use some interpretations that also work for the non-binary case:
    # if the ASTNode is a logical AND, call evaluateContext for all children and return the conjunction of the child results
    if math_AST.getType() == libsbml.AST_LOGICAL_AND:
        return( all( [evaluateContext(source_ts_dict, math_AST.getChild(i)) for i in range(0, math_AST.getNumChildren())]) )
    # similarly, if it is a logical OR, return the disjunction
    elif math_AST.getType() == libsbml.AST_LOGICAL_OR:
        return( any( [evaluateContext(source_ts_dict, math_AST.getChild(i)) for i in range(0, math_AST.getNumChildren())]) )
    # for NOT, return True only if not all child results are True (interpretation: NOT (A AND B AND C ...))
    elif math_AST.getType() == libsbml.AST_LOGICAL_NOT:
        return( not all( [evaluateContext(source_ts_dict, math_AST.getChild(i)) for i in range(0, math_AST.getNumChildren())]) )
    # for XOR return True only if exactly one child result is True
    elif math_AST.getType() == libsbml.AST_LOGICAL_XOR:
        return( sum( [evaluateContext(source_ts_dict, math_AST.getChild(i)) for i in range(0, math_AST.getNumChildren())]) == 1 )
    # if the ASTNode is relational
    elif math_AST.getType() in [libsbml.AST_RELATIONAL_EQ, libsbml.AST_RELATIONAL_GEQ, libsbml.AST_RELATIONAL_GT, libsbml.AST_RELATIONAL_LEQ, libsbml.AST_RELATIONAL_LT, libsbml.AST_RELATIONAL_NEQ]:
        # flip the child ASTNode tree if the structure is ( Integer (relation) source Id )
        left = math_AST.getLeftChild()
        right = math_AST.getRightChild()
        flipped = False
        if left.getType() == libsbml.AST_INTEGER and right.getType() == libsbml.AST_NAME:
            tmp = left
            left = right
            right = tmp
            flipped = True
        # if ASTNodes are now correctly ordered ( source Id (relation) Integer ), evaluate the corresponding equation/inequality
        if left.getType() == libsbml.AST_NAME and right.getType() == libsbml.AST_INTEGER:
            source = left.getName()
            int_value = right.getInteger()
            if math_AST.getType() == libsbml.AST_RELATIONAL_EQ:
                return(source_ts_dict[source] == int_value)

            elif math_AST.getType() == libsbml.AST_RELATIONAL_NEQ:
                return(source_ts_dict[source] != int_value)

            elif (math_AST.getType() == libsbml.AST_RELATIONAL_GEQ and not flipped) or (math_AST.getType() == libsbml.AST_RELATIONAL_LEQ and flipped):
                return(source_ts_dict[source] >= int_value )

            elif (math_AST.getType() == libsbml.AST_RELATIONAL_LEQ and not flipped) or (math_AST.getType() == libsbml.AST_RELATIONAL_GEQ and flipped):
                return(source_ts_dict[source] <= int_value)

            elif (math_AST.getType() == libsbml.AST_RELATIONAL_GT and not flipped) or (math_AST.getType() == libsbml.AST_RELATIONAL_LT and flipped):
                return(source_ts_dict[source] > int_value)

            elif (math_AST.getType() == libsbml.AST_RELATIONAL_LT and not flipped) or (math_AST.getType() == libsbml.AST_RELATIONAL_GT and flipped):
                return(source_ts_dict[source] < int_value)
        # If 'leaf subtrees' are not ( Integer (relation) source Id ) or ( source Id (relation) Integer ), throw an exception
        else:
            raise ParseASTError("[Error] evaluateContext: Unexpected mathML expression in FunctionTerm: Lowest level expressions must be species~integer comparisons.\nComponent species may only be compared to integer values, and only by the following relations: =, /=, >=, <=, >, < \n")
    # if any non-leaf ASTNode is not logical or relational, throw an exception
    else:
        raise ParseASTError("[Error] evaluateContext: Unexpected mathML expression in FunctionTerm: Only logical and relational mathML operators are allowed.")

#end evaluateContext




'''
Function writeSBMLToDBModel
    Main function, reads a qualSBML document and imports the model as a TREMPPI database
Input parameters:
    1) database_path: File path for the TREMPPI model database; existing files/databases at this location will be deleted.
    2) sbml_input_path: File path to the qualSBML input file.
    3) use_species_name_attribute: If True, the function will try to use the name attribute of qualSBML Qualitative Species, instead of the Id attribute.
        Will throw an appropriate exception if not successful.
    4) print_warnings: If True, warnings for non-critical problems will be printed
Output:
    If qualSBML file is successfully parsed, a model database is created at database_path
'''
def writeSBMLToDBModel(database_path, sbml_input_path, use_species_name_attribute=False, print_warnings=True):

    #print("Database connection established!\n")
    # database connection: remove existing files from database_path and create a new database and corresponding connection
    try:
        if os.path.isfile(database_path):
            os.remove(database_path)
        conn = sqlite3.connect(database_path)
    except sqlite3.OperationalError as e:
        raise Exception("[Error] writeSBMLToDBModel; {0} : Could not create database at this location.\n{1}".format(database_path, e.value))

    try:
        _writeSBMLToDBModel(conn, database_path, sbml_input_path, use_species_name_attribute, print_warnings)
    except Exception as e:
        conn.close()
        raise  e

    conn.close()

def _writeSBMLToDBModel(conn, database_path, sbml_input_path, use_species_name_attribute, print_warnings):
    c = conn.cursor()
    # read file
    if not os.path.exists(sbml_input_path):
        raise Exception("[Error] {0} : no such file.\n".format(sbml_input_path))
    # try to parse SBML document
    try:
        document = libsbml.readSBML(sbml_input_path)
    except libsbml.SBMLError as e:
        raise Exception("[Error] writeSBMLToDBModel; {0} : File not readable by method libsbml.readSBML(), aborting model import.\n {1}".format(sbml_input_path, e.value))
    # check SBML level
    if not (document.getLevel() == 3 and document.getVersion() == 1) and print_warnings:
        print("[Warning] writeSBMLToDBModel; {0} : SBML file has level {0}.{1}. This tool was written for SBML Level 3.1.\n".format(document.getLevel(), document.getVersion()))
    # check if qualitative model
    if not document.getPkgRequired("qual") and print_warnings:
        print("[Warning] writeSBMLToDBModel; {0} : \"qual\"-package is not set as required for this SBML file.\n".format(sbml_input_path))
    # check model consistency (document.checkConsistency() is called automatically by libsbml.readSBML)
    errors = document.getNumErrors()
    seriousErrors = False
    if errors > 0:
        for i in range(errors):
            severity = document.getError(i).getSeverity()
            if (severity == libsbml.LIBSBML_SEV_ERROR) or (severity == libsbml.LIBSBML_SEV_FATAL):
                seriousErrors = True
        if seriousErrors:
            raise Exception("[Error] writeSBMLToDBModel; {0} : SBML file invalid or inconsistant (fatal errors), aborting model import.\n Error Log from libSBML:\n{1}".format(sbml_input_path, document.getErrorLog().toString()))
        elif print_warnings:
            print("[Warning] writeSBMLToDBModel; {0} : SBML file inconsistancies (non-fatal errors).\n Error Log from libSBML:\n{1}".format(sbml_input_path, document.getErrorLog().toString()))

    # create a model object
    model = document.getModel()
    # get a QualModelPlugin object plugged into the model object (enables qualSBML interface).
    mplugin = model.getPlugin("qual")


    # Selection and checks of species names to be used for the TREMPPI database:
    # Since the SBML exporter tool (db2sbmp.py) does not work with component names with underscores "_" and the QualitativeSpecies:Name attribute is not guaranteed to be unique,
    # the existance and uniqueness of the names to be used internally by TREMPPI has to be checked.
    species_id_name_dict = {}   # dictionary with {SBML QS:Id:(Name for TREMPPI)} pairs for database inserts. Unmodified Id is still used by the function internally, since SBML FunctionTerms use QS:Id as species identifiers.
    species_modNames = []       # Names (SBML QS:Name or SBML QS:Id) without underscores
    # for all qualitativeSpecies
    for qs in mplugin.getListOfQualitativeSpecies():
        # if name attribute should be used, remove name underscores and fill Id:name dictionary
        if use_species_name_attribute and qs.isSetName():
            species_modNames.append(re.sub('_+', '', qs.getName()))
            species_id_name_dict.update({qs.getId():re.sub('_+', '', qs.getName())})
        # if names should be used but are not all set, throw an exception
        elif use_species_name_attribute:
            raise Exception("[Error] writeSBMLToDBModel; {0} : SBML Qualitative species name attributes are not set for all species, aborting model import.\n\tTry using species Ids instead by setting use_species_name_attributes=False.".format(sbml_input_path))
        # if Ids should be used, remove Id underscores and fill Id:(Id without underscores) dictionary
        else:
            species_modNames.append(re.sub('_+', '', qs.getId()))
            species_id_name_dict.update({qs.getId():re.sub('_+', '', qs.getId())})
    # if selected names are not unique, throw an exception
    if use_species_name_attribute and len(set(species_modNames)) != len(species_modNames):
        raise Exception("[Error] writeSBMLToDBModel; {0} : SBML Qualitative species name attributes are not unique without underscores, aborting model import.\n\tTry using species Ids instead by setting use_species_name_attributes=False.".format(sbml_input_path))
    elif len(set(species_modNames)) != len(species_modNames):
        raise Exception("[Error] writeSBMLToDBModel; {0} : SBML Qualitative species Id attributes are not unique without underscores, aborting model import.\n\tTry using species names instead by setting use_species_name_attributes=True.".format(sbml_input_path))

    # Check if maximum activity level is set for each species in SBML. If not, a boolean model is assumed.
    # Also fill two dictionaries with MaxActivity by Name and Id
    MA_is_set = True
    species_Id_maxActivity_dict = {}        # used by function computeThresholds
    species_Name_maxActivity_dict = {}      # used to order Name - MaxActivity pairs by Name (necessary if Id attributes are not alphabetically ordered or the qual. species name attribute is used)
    for qs in mplugin.getListOfQualitativeSpecies():
        # Use Name or Id as component name
        if use_species_name_attribute:
            Name = re.sub('_+', '', qs.getName())
        else:
            Name = re.sub('_+', '', qs.getId())
        # Use maxLevel or 1 as MaxActivity
        if qs.isSetMaxLevel():
            MaxActivity = qs.getMaxLevel()
        else:
            MaxActivity = 1
            MA_is_set = False
        species_Id_maxActivity_dict.update({qs.getId():MaxActivity})
        species_Name_maxActivity_dict.update({Name:MaxActivity})

    if not MA_is_set and print_warnings:
        print("[Warning] writeSBMLToDBModel; {0} : Maximum activity level not set in SBML file for at least one species. Assuming max. level = 1.\n".format(sbml_input_path))

    # Table Components
    c.execute('CREATE TABLE Components (Name TEXT, MaxActivity INTEGER)')
    # fill table Components in alphabetical order
    for Name in sorted(species_Name_maxActivity_dict.keys()):
        c.execute('INSERT INTO Components VALUES (?,?)', (Name, species_Name_maxActivity_dict[Name]))

    # Create temporary table Regulations, needs to be ordered later.
    c.execute('CREATE TABLE TmpRegulations (Source TEXT, Target TEXT, Threshold INTEGER)')

    # Fill table Regulations and extract regulatory contexts and target values from qualSBML transitions
    input_transition_effect_is_OK = True    # qualitative models should have specific input and output transition effect values
    output_transition_effect_is_OK = True
    context_list = []       # list for regulatory context string representations (e.g. 'K_raf_01')
    target_value_list = []  # list for corresponding context target values
    # for each qualSBML transition t:
    for t in mplugin.getListOfTransitions():

        # sources and targets of this transition (2 lists of RefID strings == Qalitative species names == TREMPPI component names)
        target_list = []
        source_list = []
        target_name_list = []
        source_name_list = []

        # get targets for table Regulations, order by target name
        for output in t.getListOfOutputs():
            target_list.append(output.getQualitativeSpecies())  # returns RefID (Qual. species Id)
            if use_species_name_attribute:
                target_name_list.append(species_id_name_dict[output.getQualitativeSpecies()])
            else:
                target_name_list.append(re.sub('_+', '', output.getQualitativeSpecies()))
            if output.getTransitionEffect() != 1:               # logical/multivalued models should have "assignmentLevel" == 1 (see qualSBML doc.)
                output_transition_effect_is_OK = False
        # order target list by internal TREMPPI names
        target_list = [target for target_name, target in sorted(zip(target_name_list, target_list))]

        # get sources for table Regulations, order by source name
        for input in t.getListOfInputs():
            source_list.append(input.getQualitativeSpecies())   # returns RefID (Qual. species Id)
            if use_species_name_attribute:
                source_name_list.append(species_id_name_dict[input.getQualitativeSpecies()])
            else:
                source_name_list.append(re.sub('_+', '', input.getQualitativeSpecies()))
            if input.getTransitionEffect() != 0:                # logical/multivalued models should have "none" == 0 (see qualSBML doc.)
                input_transition_effect_is_OK = False
        # order source list by internal TREMPPI names
        source_list = [source for source_name, source in sorted(zip(source_name_list, source_list))]

        # dictionary for storage of thresholds for each source species in this transition
        source_threshold_dict = {s:[] for s in source_list}     # e.g. {raf:[1,3], rafB:[1]}

        # find thresholds of sources for the TREMPPI regulations which are encoded in this qualSBML transition's function terms
        for f in t.getListOfFunctionTerms():
            # get the ASTNode tree of the SBML mathML definition of the function term
            # getMath() returns the top ASTNode of the tree
            math_AST = f.getMath()
            try:
                # source_threshold_dict is progressively filled/updated with information from each SBML FunctionTerm element
                computeThresholds(math_AST, source_threshold_dict, species_Id_maxActivity_dict)
            except ParseASTError as e:
                raise Exception("[Error] writeSBMLToDBModel;  {0} : Couldn't compute Thresholds for FunctionTerm of target {1} (SBML document, line {2}). Aborting model import. \n{3}".format(sbml_input_path, target_list[0], f.getLine(), e.value))

        # fill table TmpRegulations with regulations from this libsbml.Transition
        for target in target_list:
            for source in source_list:
                for threshold in source_threshold_dict[source]:
                    c.execute('INSERT INTO TmpRegulations VALUES (?,?,?)', (species_id_name_dict[source], species_id_name_dict[target], threshold))

        # create regulatory context definitions and evaluate libsbml.FunctionTerm math for context threshold states
        for target in target_list:
            # build a list of lists, where each sublist contains 0 and all thresholds for one input/source species
            thresholds_list_with_lower_zeros = [[0] + source_threshold_dict[s] for s in source_list]
            # create a list of all possible source threshold state combinations for this transition
            # e.g. CompA thresholds = [1], CompB thresholds = [1,2] -> context_threshold_states = [ [0,0], [0,1], [0,2], [1,0], [1,1], [1,2] ]
            context_threshold_states = list(itertools.product(*thresholds_list_with_lower_zeros))
            # for each context threshold state
            for cts in context_threshold_states:
                # get the target name to be used for this TREMPPI context definition (name attribute or Id without underscores)
                target_db_name = species_id_name_dict[target]
                # build the TREMPPI regulatory context definition (e.g. K_Raf_01) and append to context list
                context = 'K_{0}_{1}'.format(target_db_name, ''.join([str(t) for t in cts]))
                context_list.append(context)
                # get the target value for this context:
                # initialize the target value with the libsbml.Transition.DefaultTerm resultLevel
                target_value = t.getDefaultTerm().getResultLevel()
                # create a dictionary with pairs source:(source threshold state) for this context threshold state (cts)
                source_ts_dict = {source:threshold_state for (source, threshold_state) in zip(source_list, cts)}
                # Call evaluateContext for each FunctionTerm until a function evaluates to true for this context or all FunctionTerms have been evaluated.
                # If a FunctionTerm evaluates to true, use its result level as the target value for this context.
                # If none evaluates to true, keep the DefaultTerm result level as the target value.
                for ft in t.getListOfFunctionTerms():
                    try:
                        if evaluateContext(source_ts_dict, ft.getMath()):
                            target_value = ft.getResultLevel()
                            break
                    except ParseASTError as e:
                        raise Exception("[Error] writeSBMLToDBModel; {0} : Couldn't evaluate context for FunctionTerm of target {1}. \n{2}".format(sbml_input_path, target_list[0], e.value))
                # store the target values in order of regulatory contexts
                target_value_list.append(target_value)

    # warn if transition effect attributes were set to unexpected values
    if not output_transition_effect_is_OK and print_warnings:
        print("[Warning] writeSBMLToDBModel; {0} : At least one Transition output species has the transitionEffect attribute set to 'production' (0). Should be 'assignment level' (1) for logical/multi-valued models.\n".format(sbml_input_path))

    if not input_transition_effect_is_OK and print_warnings:
        print("[Warning] writeSBMLToDBModel; {0} : At least one Transition input species has the transitionEffect attribute set to 'consumption' (1). Should be 'none' (0) for logical/multi-valued models.\n".format(sbml_input_path))

    # Create final table Regulations, ordered by Target(1), Source(2), Threshold(3) and discard temporary table
    c.execute('CREATE TABLE Regulations (Source TEXT, Target TEXT, Threshold INTEGER)')
    c.execute('INSERT INTO Regulations SELECT * FROM TmpRegulations ORDER BY Target, Source, Threshold')
    c.execute('DROP TABLE TmpRegulations')

    # Table Parametrizations
    # build string for definition of column names (context names)
    column_def_string = ' INTEGER, '.join(sorted(context_list)) + ' INTEGER'
    # build string for context target values, ordered by context names
    target_value_string = ', '.join([str(t) for c, t in sorted(zip(context_list, target_value_list))])
    # create table and insert target values
    c.execute('CREATE TABLE Parametrizations ({0})'.format(column_def_string))
    c.execute('INSERT INTO Parametrizations VALUES ({0})'.format(target_value_string))
    # commit database changes
    conn.commit()

    #end writeSBMLToDBModel



## Example models

# SBML to DB import example 1: Cortical Area Development
#writeSBMLToDBModel("./data/databases/modelDB.sqlite", "./data/sbml/CorticalAreaDevelopment_cellCollective_boolean.sbml")
#writeSBMLToDBModel("./data/databases/modelDB.sqlite", "./data/sbml/CorticalAreaDevelopment_cellCollective_boolean.sbml", use_species_name_attribute=True)

# SBML to DB import example 2: Mammalian Cellcycle
#writeSBMLToDBModel("./data/databases/modelDB.sqlite", "./data/sbml/MammalianCellCycle_cellCollective_boolean.sbml")
#writeSBMLToDBModel("./data/databases/modelDB.sqlite", "./data/sbml/MammalianCellCycle_cellCollective_boolean.sbml", use_species_name_attribute=True)

# SBML to DB import example 3: Notch Pathway
#writeSBMLToDBModel("./data/databases/modelDB.sqlite", "./data/sbml/NotchPathway_ginsim_multiValued.sbml", use_species_name_attribute=False)
# The notch Pathway model does not have Species name Attributes set, as such running the tool with this line will fail and throw an exception.
#writeSBMLToDBModel("./data/databases/modelDB.sqlite", "./data/sbml/NotchPathway_ginsim_multiValued.sbml", use_species_name_attribute=True)

# read from last output file of the export function "writeDBModelToSBML"
#writeSBMLToDBModel("./data/databases/modelDB2.sqlite", "./data/sbml/db2sbml_output.sbml")