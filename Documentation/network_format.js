/* 
* This file provide a structure description of a model.
* Also it serves as a model of a simple loop
*/  
var model = {
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
					"Label" : "ActivatingOnly" // 
				}
			}
		]
	}
}
