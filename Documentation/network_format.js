/** MODEL:
 * This file provide a structure description of a model.
 * Also it serves as a model of a simple loop
 */  
var network = {
   "elements" : {
		"nodes" : [ // Comma separated list of nodes
			{
				"data" : {
					"id" : "0", // Components must have IDs from 0 to n-1
					"Name" : "A", // Name of the component, must start with a letter and must contain only letters, numbers and a underscore
					"MaxActivity" : 1 // Maximal activity level of a component, must be between 1 and 9
				}
			}
		]
		"edges" : [ // Comma separated list of edges
			{
				"data" : {
					"source" : "0", // ID of the source
					"target" : "1", // ID of the target
					"Threshold" : 1, // a threshold of the regulation
					"Label" : "ActivatingOnly", // A label denoting the edge function, see below
					"Constraints" : [ // A comma separated list of constraints, see below
						"B:2<2"
					]
				}
			}
		]
	}
}

/** EDGE LABELS:
 * Edge label can be one of:
 * Activating ~ positive
 * ActivatingOnly ~ monotone positive
 * Inhibiting ~ negative
 * InhibitingOnly ~ monotone negative
 * NotActivating ~ monotone and not positive
 * NotInhibiting ~ monotone and not negative
 * Observable ~ positive or negative
 * NotObservable ~ not positive and not negative 
 * Free ~ all is possible
 * Also any boolean expression over the variables "+" and "-" for positive and negative respectively, e.g. "+&!-" for Activating only. See expressions section for more details. 
 */
 
 /** CONSTRAINTS:
  * A constraint is an expression of the form "context x n" or "context x context" where
  * "x" is one of {!=, <, <=, =, >, >=}
  * n is a single digit positive integer, i.e. a number in {0,...,9}
  * context is a comma separated list of regulations where a regulator is denoted as "name:threshold"		
  */
 
/** EXPRESSIONS:
 * logical expressions are given by the recursive recipe:
 * - any variable is an expression
 * - "tt" and "ff" are expression for true and false respectively 
 * - for "a" a expression, "!a" is an expression, denoting negation
 * - for "a","b" expressions, "a&b" and "a|b" are expressions, denoting conjunction and disjunction respectively
 * - nothing else is a variable
 */
 