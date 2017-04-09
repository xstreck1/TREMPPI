
#import sys
import os.path
import sqlite3
import libsbml
#from libsbml import *
import re
import itertools




class ParseASTError(Exception):
    def __init__(self, value):
        self.value = value
    def __str__(self):
        return repr(self.value)




def computeThresholds(math_AST, sources_dict, species_Id_maxActivity_dict):

    if math_AST.getType() in [libsbml.AST_LOGICAL_AND, libsbml.AST_LOGICAL_OR, libsbml.AST_LOGICAL_NOT, libsbml.AST_LOGICAL_XOR]:
        #print("numchildren: {0}".format(math_AST.getNumChildren()))
        for i in range(0, math_AST.getNumChildren()):
            #print("Logical {0}:".format(i))
            sources_dict = computeThresholds(math_AST.getChild(i), sources_dict, species_Id_maxActivity_dict)

    elif math_AST.getType() in [libsbml.AST_RELATIONAL_EQ, libsbml.AST_RELATIONAL_GEQ, libsbml.AST_RELATIONAL_GT, libsbml.AST_RELATIONAL_LEQ, libsbml.AST_RELATIONAL_LT, libsbml.AST_RELATIONAL_NEQ]:
        left = math_AST.getLeftChild()
        right = math_AST.getRightChild()
        flipped = False

        if left.getType() == libsbml.AST_INTEGER and right.getType() == libsbml.AST_NAME:
            tmp = left
            left = right
            right = tmp
            flipped = True

        if left.getType() == libsbml.AST_NAME and right.getType() == libsbml.AST_INTEGER:
            source = left.getName()
            #print("Source: ", source)
            int_value = right.getInteger()
            if math_AST.getType() == libsbml.AST_RELATIONAL_EQ or math_AST.getType() == libsbml.AST_RELATIONAL_NEQ:
                new_thresholds = [x for x in [int_value, int_value + 1] if x not in sources_dict[source] and x != 0 and x <= species_Id_maxActivity_dict[source]]
                sources_dict[source].extend(new_thresholds)
            elif (math_AST.getType() == libsbml.AST_RELATIONAL_GEQ and not flipped) or (math_AST.getType() == libsbml.AST_RELATIONAL_LEQ and flipped):
                if int_value not in sources_dict[source] and int_value != 0:
                    sources_dict[source].append(int_value)
            elif (math_AST.getType() == libsbml.AST_RELATIONAL_LEQ and not flipped) or (math_AST.getType() == libsbml.AST_RELATIONAL_GEQ and flipped):
                if int_value + 1 not in sources_dict[source] and int_value + 1 <= species_Id_maxActivity_dict[source]:
                    sources_dict[source].append(int_value + 1)
            elif (math_AST.getType() == libsbml.AST_RELATIONAL_GT and not flipped) or (math_AST.getType() == libsbml.AST_RELATIONAL_LT and flipped):
                if int_value + 1 not in sources_dict[source] and int_value + 1 <= species_Id_maxActivity_dict[source]:
                    sources_dict[source].append(int_value + 1)
            elif (math_AST.getType() == libsbml.AST_RELATIONAL_LT and not flipped) or (math_AST.getType() == libsbml.AST_RELATIONAL_GT and flipped):
                if int_value not in sources_dict[source] and int_value != 0:
                    sources_dict[source].append(int_value)

    else:
        raise ParseASTError("[Error] computeThresholds: Unexpected mathML expression in FunctionTerm: Only logical and relational mathML operators are allowed.\n\tLowest level expressions must be species~integer comparisons.\n\tComponent species may only be compared to integer values, and only by the following relations: eq/, neq/, geq/, leq/, gt/, lt/. \n")


    #print(sources_dict)
    return(sources_dict)

#end computeThresholds






def evaluateContext(source_ts_dict, math_AST):

    if math_AST.getType() == libsbml.AST_LOGICAL_AND:
        return( all( [evaluateContext(source_ts_dict, math_AST.getChild(i)) for i in range(0, math_AST.getNumChildren())]) )

    elif math_AST.getType() == libsbml.AST_LOGICAL_OR:
        return( any( [evaluateContext(source_ts_dict, math_AST.getChild(i)) for i in range(0, math_AST.getNumChildren())]) )

    elif math_AST.getType() == libsbml.AST_LOGICAL_NOT:
        return( not all( [evaluateContext(source_ts_dict, math_AST.getChild(i)) for i in range(0, math_AST.getNumChildren())]) )

    elif math_AST.getType() == libsbml.AST_LOGICAL_XOR:
        return( sum( [evaluateContext(source_ts_dict, math_AST.getChild(i)) for i in range(0, math_AST.getNumChildren())]) == 1 )

    elif math_AST.getType() in [libsbml.AST_RELATIONAL_EQ, libsbml.AST_RELATIONAL_GEQ, libsbml.AST_RELATIONAL_GT, libsbml.AST_RELATIONAL_LEQ, libsbml.AST_RELATIONAL_LT, libsbml.AST_RELATIONAL_NEQ]:

        left = math_AST.getLeftChild()
        right = math_AST.getRightChild()
        flipped = False
        if left.getType() == libsbml.AST_INTEGER and right.getType() == libsbml.AST_NAME:
            tmp = left
            left = right
            right = tmp
            flipped = True

        if left.getType() == libsbml.AST_NAME and right.getType() == libsbml.AST_INTEGER:
            source = left.getName()
            #print("Source: ", source)
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

        else:
            raise ParseASTError("[Error] evaluateContext: Unexpected mathML expression in FunctionTerm: Lowest level expressions must be species~integer comparisons.\nComponent species may only be compared to integer values, and only by the following relations: =, /=, >=, <=, >, < \n")

    else:
        raise ParseASTError("[Error] evaluateContext: Unexpected mathML expression in FunctionTerm: Only logical and relational mathML operators are allowed.")

#end evaluateContext






'''
Function writeSBMLToDBModel
Input parameters:
    1) database_path: url where the TREMPPI model database is to be created; existing files/databases at this url will be deleted
    2) sbml_input_path: url of the qualSBML input file
Output:
    0 for successfully parsed qualSBML file
    1 in case of errors
    If qualSBML file is successfully parsed, a model database is created at database_path
'''
def writeSBMLToDBModel(database_path, sbml_input_path, use_species_name_attribute=False):

    # read file
    if not os.path.exists(sbml_input_path):
        print("[Error] {0} : no such file.\n".format(sbml_input_path))
        return 1
    # try to parse SBML document
    try:
        document = libsbml.readSBML(sbml_input_path)
    except:
        print("[Error] {0} : File not readable by method libsbml.readSBML(), aborting model import.\n".format(sbml_input_path))
        return 1
    # check SBML level
    if not (document.getLevel() == 3 and document.getVersion() == 1):
        print("[Warning] {0} : SBML file has level {0}.{1}. This tool was written for SBML Level 3.1.\n".format(document.getLevel(), document.getVersion()))
    # check if qualitative model
    if not document.getPkgRequired("qual"):
        print("[Warning] {0} : \"qual\"-package is not set as required for this SBML file.\n".format(sbml_input_path))
    # check model consistency (document.checkConsistency() is called automatically by libsbml.readSBML)
    errors = document.getNumErrors()
    seriousErrors = False
    if errors > 0:
        for i in range(errors):
            severity = document.getError(i).getSeverity()
            if (severity == libsbml.LIBSBML_SEV_ERROR) or (severity == libsbml.LIBSBML_SEV_FATAL):
                seriousErrors = True
        if seriousErrors:
            print("[Error] {0} : SBML file invalid or inconsistant (fatal errors), aborting model import.\n Error Log from libSBML:\n{1}".format(sbml_input_path, document.getErrorLog().toString()))
            return 1
        else:
            print("[Warning] {0} : SBML file inconsistancies (non-fatal errors).\n Error Log from libSBML:\n{1}".format(sbml_input_path, document.getErrorLog().toString()))

    # create a model object
    model = document.getModel()
    # get a QualModelPlugin object plugged into the model object (enables qualSBML interface).
    mplugin = model.getPlugin("qual")

    #print("SBML file read successfully!\n")

    # database connection
    try:
        if os.path.isfile(database_path):
            os.remove(database_path)
        conn = sqlite3.connect(database_path)
        c = conn.cursor()
    except sqlite3.OperationalError:
        print("[Error] {0} : Could not create database at this location.\n".format(database_path))
        return 1

    #print("Database connection established!\n")

    # Selection and checks of species names to be used for the TREMPPI database:
    # Since the SBML exporter tool (db2sbmp.py) does not work with component names with underscores "_" and the QualitativeSpecies:Name attribute is not guaranteed to be unique,
    # the existance and uniqueness of the names to be used internally by TREMPPI has to be checked.
    species_id_name_dict = {}   # dictionary with {SBML QS:Id:(Name for TREMPPI)} pairs for database inserts. Unmodified Id is still used by the function internally, since SBML FunctionTerms use QS:Id as species identifiers.
    species_modNames = []       # Names (SBML QS:Name or SBML QS:Id) without underscores
    for qs in mplugin.getListOfQualitativeSpecies():
        if use_species_name_attribute and qs.isSetName():
            species_modNames.append(re.sub('_+', '', qs.getName()))
            species_id_name_dict.update({qs.getId():re.sub('_+', '', qs.getName())})
        elif use_species_name_attribute:
            print("[Error] {0} : SBML Qualitative species name attributes are not set for all species, aborting model import.\n\tTry using species Ids instead by setting use_species_name_attributes=False.".format(sbml_input_path))
            return 1
        else:
            species_modNames.append(re.sub('_+', '', qs.getId()))
            species_id_name_dict.update({qs.getId():re.sub('_+', '', qs.getId())})
    if use_species_name_attribute and len(set(species_modNames)) != len(species_modNames):
        print("[Error] {0} : SBML Qualitative species name attributes are not unique without underscores, aborting model import.\n\tTry using species Ids instead by setting use_species_name_attributes=False.".format(sbml_input_path))
        return 1
    elif len(set(species_modNames)) != len(species_modNames):
        print("[Error] {0} : SBML Qualitative species Id attributes are not unique without underscores, aborting model import.\n\tTry using species names instead by setting use_species_name_attributes=True.".format(sbml_input_path))
        return 1

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

    if not MA_is_set:
        print("[Warning] {0} : Maximum activity level not set in SBML file for at least one species. Assuming max. level = 1.\n".format(sbml_input_path))

    # Table Components
    c.execute('CREATE TABLE Components (Name TEXT, MaxActivity INTEGER)')

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

        # dictionary for storage of thresholds for each source in this transition
        source_threshold_dict = {s:[] for s in source_list}     # e.g. {raf:[1,3], rafB:[1]}

        # find thresholds of sources for the TREMPPI regulations which are encoded in this qualSBML transition's function terms
        for f in t.getListOfFunctionTerms():
            # get the ASTNode tree of the SBML mathML definition of the function term
            # getMath() returns the top ASTNode of the tree
            math_AST = f.getMath()
            try:
                # source_threshold_dict is progressively filled/updated with information from each SBML FunctionTerm element
                source_threshold_dict = computeThresholds(math_AST, source_threshold_dict, species_Id_maxActivity_dict)
            except ParseASTError as e:
                print("[Error] {0} : Couldn't compute Thresholds for FunctionTerm of target {2} (line {1}). Aborting model import. \n{3}".format(sbml_input_path, f.getLine(), target_list[0], e.value))
                return 1

        #print('std: ', source_threshold_dict)
        #print("filling table Regulations, source_threshold_dict={0}, target_list={1}, source_list={2}".format(source_threshold_dict,target_list,source_list))
        for target in target_list:
            for source in source_list:
                #print(source_threshold_dict[source])
                for threshold in source_threshold_dict[source]:
                    c.execute('INSERT INTO TmpRegulations VALUES (?,?,?)', (species_id_name_dict[source], species_id_name_dict[target], threshold))

        for target in target_list:
            thresholds_list_with_lower_zeros = [[0] + source_threshold_dict[s] for s in source_list]
            context_threshold_states = list(itertools.product(*thresholds_list_with_lower_zeros))
            #print('cts: ', context_threshold_states)
            for cts in context_threshold_states:

                if use_species_name_attribute:
                    target_db_name = species_id_name_dict[target]
                else:
                    target_db_name = re.sub('_+', '', target)
                context = 'K_{0}_{1}'.format(target_db_name, ''.join([str(t) for t in cts]))
                context_list.append(context)

                target_value = t.getDefaultTerm().getResultLevel()
                source_ts_dict = {source:threshold_state for (source, threshold_state) in zip(source_list, cts)}
                for ft in t.getListOfFunctionTerms():
                    try:
                        if evaluateContext(source_ts_dict, ft.getMath()):
                            target_value = ft.getResultLevel()
                            break
                    except ParseASTError as e:
                        print("[Error] {0} : Couldn't evaluate context for FunctionTerm of target {1}. \n{2}".format(sbml_input_path, target_list[0], e.value))
                        return 1
                target_value_list.append(target_value)

    # create final table Regulations
    c.execute('CREATE TABLE Regulations (Source TEXT, Target TEXT, Threshold INTEGER)')
    c.execute('INSERT INTO Regulations SELECT * FROM TmpRegulations ORDER BY Target, Source, Threshold')
    c.execute('DROP TABLE TmpRegulations')

    #print('cl: ', context_list)
    #print('tv: ', target_value_list)
    # Table Parametrizations
    column_def_string = ' INTEGER, '.join(sorted(context_list)) + ' INTEGER'
    target_value_string = ', '.join([str(t) for c, t in sorted(zip(context_list, target_value_list))])
    c.execute('CREATE TABLE Parametrizations ({0})'.format(column_def_string))
    c.execute('INSERT INTO Parametrizations VALUES ({0})'.format(target_value_string))

    conn.commit()
    return(0)

#end writeSBMLToDBModel


# read from own last output sbml file
#writeSBMLToDBModel("./data/databases/modelDB2.sqlite", "./data/sbml/db2sbml_output.sbml")

# CorticalAreaDevelopment
#writeSBMLToDBModel("./data/databases/modelDB.sqlite", "./data/sbml/CorticalAreaDevelopment_cellCollective_boolean.sbml")
#writeSBMLToDBModel("./data/databases/modelDB.sqlite", "./data/sbml/CorticalAreaDevelopment_cellCollective_boolean.sbml", use_species_name_attribute=True)

# MammalianCellCycle
#writeSBMLToDBModel("./data/databases/modelDB.sqlite", "./data/sbml/MammalianCellCycle_cellCollective_boolean.sbml")
#writeSBMLToDBModel("./data/databases/modelDB.sqlite", "./data/sbml/MammalianCellCycle_cellCollective_boolean.sbml", use_species_name_attribute=True)


# NotchPathway
#writeSBMLToDBModel("./data/databases/modelDB.sqlite", "./data/sbml/NotchPathway_ginsim_multiValued.sbml", use_species_name_attribute=False)
#writeSBMLToDBModel("./data/databases/modelDB.sqlite", "./data/sbml/NotchPathway_ginsim_multiValued.sbml", use_species_name_attribute=True)
