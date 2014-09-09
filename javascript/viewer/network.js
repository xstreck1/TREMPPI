var elements = {
	"nodes": [
		{
			"data": {
				"Name": "A",
				"MaxActivity": 1,
				"id": "n0"
			},
			"position": {
				"x": 399.1952662721893,
				"y": 157.67455621301775
			},
			"group": "nodes",
			"removed": false,
			"selected": false,
			"selectable": true,
			"locked": false,
			"grabbed": false,
			"grabbable": true,
			"classes": ""
		},
		{
			"data": {
				"Name": "C",
				"MaxActivity": 2,
				"Constraint": "B:2<2",
				"id": "n1"
			},
			"position": {
				"x": 477.4319526627219,
				"y": 225.7396449704142
			},
			"group": "nodes",
			"removed": false,
			"selected": false,
			"selectable": true,
			"locked": false,
			"grabbed": false,
			"grabbable": true,
			"classes": ""
		}
	],
	"edges": [
		{
			"data": {
				"source": "n0",
				"target": "n1",
				"Threshold": 1,
				"Label": "tt",
				"id": "e0"
			},
			"position": {},
			"group": "edges",
			"removed": false,
			"selected": false,
			"selectable": true,
			"locked": false,
			"grabbed": false,
			"grabbable": true,
			"classes": ""
		},
		{
			"data": {
				"source": "n1",
				"target": "n1",
				"Threshold": 2,
				"Label": "+",
				"id": "e1"
			},
			"position": {},
			"group": "edges",
			"removed": false,
			"selected": false,
			"selectable": true,
			"locked": false,
			"grabbed": false,
			"grabbable": true,
			"classes": ""
		}
	]
};