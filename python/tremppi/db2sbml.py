# SQLite3 database
import sqlite3
# libsbml interface
import libsbml
# regular expressions
import re


'''
Function buildASTInequality
Input parameters:
    1) source (string): Name of the source component.
    2) relation (string): 'lt' for less than or 'geq' for greater or equal than.
    3) threshold (integer): Value of the component threshold/max activity to be compared to.
Output:
    The ASTNode tree for this inequality: (source ( relation ) threshold)
'''
def buildASTInequality(source, relation, threshold):
    # create a ASTNode for the source/regulator
    source_ASTNode = libsbml.ASTNode(libsbml.AST_NAME)
    source_ASTNode.setName(source)

    # create relation ASTNode. This exporter only uses '<' (lt) and '>=' (geq) relations
    if(relation == 'lt'):
        relation_ASTNode = libsbml.ASTNode(libsbml.AST_RELATIONAL_LT)
    elif(relation == 'geq'):
        relation_ASTNode = libsbml.ASTNode(libsbml.AST_RELATIONAL_GEQ)

    # create threshold ASTNode
    threshold_ASTNode = libsbml.ASTNode(libsbml.AST_INTEGER)  # Integer ASTNode for threshold
    threshold_ASTNode.setValue(threshold)

    relation_ASTNode.addChild(libsbml.ASTNode(source_ASTNode))  # add source and upper bound nodes as children of the inequality node
    relation_ASTNode.addChild(threshold_ASTNode)

    return relation_ASTNode

# end buildInequalityASTTree


'''
Function writeDBModelToSBML
    Main function, exports a TREMPPI model as a qualSBML file.
Input parameters:
    1) database_path (string): File path of the TREMPPI model database.
    2) sbml_output_path (string): File path for the qualSBML output file.
    3) modelRow (integer): The row of database table "Parametrizations" to be exported as qualSBML.
    Row indexing starts at 1 rather than 0. (Default setting: 1)
Output:
    If no error occurs, a qualSBML file of the specified model is created at sbml_output_path.
'''
def writeDBModelToSBML(database_path, sbml_output_path, modelRow=1):

    # Create the connection to the SQLite model database
    try:
        conn = sqlite3.connect(database_path)
    except sqlite3.OperationalError as e:
        conn.close()
        raise Exception("[Error] writeDBModelToSBML, {0} : Could not connect to database at this location.\n".format(database_path)) from e

    try:
        _writeDBModelToSBML(conn, database_path, sbml_output_path, modelRow)
    except Exception as e:
        conn.close()
        raise e

    conn.close()


def _writeDBModelToSBML(conn, database_path, sbml_output_path, modelRow):
    c = conn.cursor()

    # check table existence
    c.execute('SELECT name FROM sqlite_master WHERE type="table"')
    db_tables = [row[0] for row in c.fetchall()]
    expected_tables = ['Components', 'Regulations', 'Parametrizations']
    if len(set(expected_tables).difference(set(db_tables))) != 0:
        raise Exception("[Error] writeDBModelToSBML, {0} : Cannot find table(s) {1} in database.".
                        format(database_path, set(expected_tables).difference(set(db_tables))))
    # check model row existence
    rowcount = c.execute("SELECT COUNT (*) FROM Parametrizations").fetchone()[0]
    if modelRow > rowcount:
        raise Exception(
            "[Error] writeDBModelToSBML, {0} : Cannot select row {1} from table Parametrizations with {2} rows. Aborting model export.\n".format(
                database_path, modelRow, rowcount))

    # Create an SBMLNamespaces object with the given SBML level, version,
    # package name, package version.
    sbmlns = libsbml.SBMLNamespaces(3, 1, "qual", 1)
    # Creates an SBMLDocument object
    document = libsbml.SBMLDocument(sbmlns)
    # mark qual as required
    document.setPackageRequired("qual", True)
    # create the Model
    model = document.createModel()

    # get a QualModelPlugin object plugged into the model object (enables qualSBML interface).
    mplugin = model.getPlugin("qual")

    # extract regulatory context descriptions from table Parametrizations
    # get the column names from table Parametrizations
    column_name_list = [tuple[0] for tuple in c.execute(
        "SELECT * FROM Parametrizations").description]  # TO DO: exception for invalid/missing model
    # create a list of all column names which represent regulatory contexts
    context_name_list = [column_name for column_name in column_name_list if
                         column_name[0:2] == 'K_']  # TO DO: index dependent = bad
    # create a list of "targets" (components with incoming regulations)
    target_list = [target for target, in
                   c.execute('SELECT DISTINCT Target FROM Regulations ORDER BY Target').fetchall()]

    # create an internal list of regulatory context descriptions
    context_list = []
    for context_name in context_name_list:
        # find the target component matching the context name
        target_match_list = [target for target in target_list if context_name[2:].find(
            target) != -1]  # can find multiple targets: e.g. context_name="K_RafB_012" will match species Raf and RafB
        target = [match for match in target_match_list if len(match) == max(map(len, target_match_list))][
            0]  # take the longest match, now "K_RafB_012" will only match RafB and not Raf
        # extract the threshold states of the regulatory context from the context name
        # regular expression pattern: find all digits following "K_RafB_" where RafB is the current context target
        pattern = r'(?<=K_{0}_)\d+'.format(target)
        # thresholds_name: string of threshold state digits (e.g. "012" for context_name K_RafB_012)
        thresholds_name = re.search(pattern, context_name).group(0)  # WILL NOT WORK for components with MaxActivity > 9
        threshold_list = [int(d) for d in thresholds_name]
        context = [context_name, target]
        context.extend(threshold_list)  # each context = list of format ['K_Raf_031', 'Raf', 0, 3, 1]
        context_list.append(context)

    # write to qualSBML model

    # set a default compartment (required for all models)
    compartment = model.createCompartment()
    compartment.setId("default")
    compartment.setConstant(True)

    # set QualitativeSpecies from Components table
    for name, maxActivity in c.execute('SELECT Name, MaxActivity FROM Components ORDER BY Name').fetchall():
        # create a qualSBML QualitativeSpecies
        qs = mplugin.createQualitativeSpecies()
        qs.setId(name)  # Id is required to be unique
        qs.setCompartment("default")
        qs.setConstant(False)  # components without inputs are usually set constant, however this is not required
        qs.setMaxLevel(maxActivity)
        qs.setName(name)  # name is not required to be set or to be unique

    # set Transitions from Regulations and Parametrizations tables
    # qualSBML Transitions have a list for input and an output QualitativeSpecies. Usually each transition only has one output (target).
    # Components with identical regulation may be described using only one Transition with multiple outputs
    # but this is not required and this function creates a transition for each target component.
    for target, in c.execute('SELECT DISTINCT Target FROM Regulations ORDER BY Target').fetchall():
        # create a Transition: one qualSBML Transition per TREMPPI component with >= 1 regulators
        t = mplugin.createTransition()
        # create qualSBML transition output for transition t
        o = t.createOutput()
        o.setQualitativeSpecies(target)
        o.setTransitionEffect(
            1)  # transitionEffect=1 means 'assignmentLevel'. Required for logical/multi valued models.

        # save a list of all regulator/input-species-names (string) for this target component
        source_list = []

        for source, in c.execute('SELECT DISTINCT Source FROM Regulations WHERE Target=? ORDER BY Source',
                                 (target,)).fetchall():
            # create qualSBML transition input for transition t
            i = t.createInput()
            i.setQualitativeSpecies(source)
            i.setTransitionEffect(0)  # transitionEffect=0 means 'none'. Required for logical/multi valued models.
            # also append this input to our regulator/input-name list
            source_list.append(source)

        # setup a ASTNode representation for  the qualSBML Transition.FunctionTerm.Math element from TREMPPI regulatory context information

        # get all regulatory contexts for this target component
        target_context_list = [context for context in context_list if context[1] == target]

        # Each regulatory context is represented in qualSBML by one Transition.FunctionTerm or by the Transition.DefaultTerm.
        # FunctionTerm definitions hold the (in-)equalities to define source/input states leading to a target/output value.
        # This function creates a FunctionTerm even for unobservable contexts as long as they are defined in table Parametrizations.
        # This leads to more FunctionTerm definitions than necessary but allows to recreate the whole network from the qualSBML file.
        for context in target_context_list:
            # list of regulator threshold states for this context
            source_tstate_list = context[2:]
            # parameter target value of this context
            param = \
            c.execute('SELECT {0} FROM Parametrizations WHERE rowid={1}'.format(context[0], modelRow)).fetchall()[0][0]

            # If this context is the context with threshold level = 0 for all regulators, use it as Transition.DefaultTerm in qualSBML
            # Transitions default to this target value if none of the FunctionTerm definitions apply to a system state
            if sum(source_tstate_list) == 0:
                # create qualSBML transition DefaultTerm for transition t
                d = t.createDefaultTerm()
                d.setResultLevel(param)  # ResultLevel is the qualSBML equivalent of a target value

            # else create a Transition.FunctionTerm term with math element
            else:
                # create qualSBML transition FunctionTerm for transition t
                f = t.createFunctionTerm()
                f.setResultLevel(param)

                # list of the ASTNode-tree inequality representations of each regulatory context
                f_ASTNode_list = []

                # find lower and upper thresholds (activity interval) of each source/regulator in this context and build an ASTNode tree of the resulting inequalities
                # for each source index
                for source_index in range(0, len(source_list)):
                    source = source_list[source_index]
                    source_tstate = source_tstate_list[
                        source_index]  # threshold state for this source (e.g. 0 for the first regulator of RafB in K_RafB_012)
                    # list of thresholds for this source -> target regulation
                    threshold_list = c.execute(
                        'SELECT Threshold FROM Regulations WHERE Target=? AND Source=? ORDER BY Threshold',
                        (target, source)).fetchall()
                    lower_t = 0  # Initialize the lower bound of the interval with 0.
                    # If the activity interval includes 0 as lower bound, it is the leftmost (lowest) activity interval.
                    # If it includes the highest threshold of the regulator as its lower bound, it is the rightmost (largest) activity interval.
                    leftmost = True  # initialize as leftmost
                    rightmost = False
                    threshold_index = 0  # index of the current regulator threshold
                    # Traverse regulator thresholds and reset lower_t until lower_t matches the threshold state in this context
                    # and set threshold_index to the index of the next higher threshold.
                    while source_tstate > lower_t:
                        lower_t = threshold_list[threshold_index][0]
                        threshold_index += 1
                        leftmost = False  # If lower_t is not 0, this is not the lowest activity interval.
                    # If this is not the rightmost (highest) interval, set upper_t to the next higher threshold of this regulator.
                    if len(threshold_list) > threshold_index:
                        upper_t = threshold_list[threshold_index][0]
                    # Otherwise, set upper_t to the maximum activity of the regulator.
                    else:
                        upper_t = c.execute('SELECT MaxActivity FROM Components WHERE Name=?', (source,)).fetchall()[0]
                        rightmost = True

                    # build ASTNode tree

                    # if this is the leftmost activity interval create inequality: source < upper bound
                    if leftmost:
                        ast = buildASTInequality(source, 'lt', upper_t)
                    # else if this is the rightmost activity interval create inequality: source >= lower bound
                    elif rightmost:
                        ast = buildASTInequality(source, 'geq', lower_t)
                    # otherwise create inequalities: (source >= lower bound) AND (source < upper bound)
                    else:
                        # create AND ASTNode
                        ast = libsbml.ASTNode(libsbml.AST_LOGICAL_AND)
                        # create (source >= lower bound) AST subtree
                        lower_t_ASTNode = buildASTInequality(source, 'geq', lower_t)
                        # create (source < upper bound) AST subtree
                        upper_t_ASTNode = buildASTInequality(source, 'lt', upper_t)
                        # add subtries to AND node
                        ast.addChild(lower_t_ASTNode)
                        ast.addChild(upper_t_ASTNode)

                    # Add the complete AST tree of this source/regulator to the list of inequality definitions for this context/FunctionTerm
                    f_ASTNode_list.append(ast)

                # If this context depends on more than one source/regulator, add an additional AND node as root of the context/FunctionTerm AST tree
                # and add all regulator subtrees as children (MathML's AND allows more than 2 children)
                if (len(f_ASTNode_list) > 1):
                    topASTNode = libsbml.ASTNode(libsbml.AST_LOGICAL_AND)
                    for ast in f_ASTNode_list:
                        topASTNode.addChild(ast)
                # otherwise make the first (and only) ASTNode in the list the root
                else:
                    topASTNode = f_ASTNode_list[0]

                # set the complete AST tree as the Math-element of this FunctionTerm
                f.setMath(topASTNode)

    # Check document consistency
    document.checkInternalConsistency()
    if document.getNumErrors() > 0:
        raise Exception(
            "[Error] writeDBModelToSBML: Exported SBML model invalid or inconsistant, aborting model export.\n Error Log from libSBML:\n{0}".format(
                document.getErrorLog().toString()))
    # Write the document to the specified output file path
    libsbml.writeSBML(document, sbml_output_path)


def database_to_editor(comps, regs):

    model_root = { 'nodes' : [], 'edges' : [] }

    id = 0
    nodeIds = {}
    for comp_name, comp_max  in comps:
        nodeIds[comp_name] = 'n' + str(id)
        model_root['nodes'].append(
            {
                'data' :
                {
                    'Name' : comp_name,
                    'MaxActivity': str(comp_max),
                    'Label' : comp_name + ':' + str(comp_max),
                    'id' : nodeIds[comp_name]
                }
            }
        )
        id += 1


    id = 0
    for source, threshold, target  in regs:
        model_root['edges'].append(
            {
                'data' :
                {
                    'source' : nodeIds[source],
                    'target' : nodeIds[target],
                    'Threshold': str(threshold),
                    'Label': 'Free',
                    'id' : 'e' + str(id)
                }
            }
        )
        id += 1

    return model_root

#writeDBModelToSBML('./data/databases/bacteriophage.sqlite', './data/sbml/db2sbml_output.sbml', modelRow=3)
#writeDBModelToSBML('./data/databases/modelDB.sqlite', './data/sbml/db2sbml_output.sbml', modelRow=1)














