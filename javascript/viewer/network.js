var elements = {
	"name": "test",
	"edges": [
		{
			"data": {
				"source": "A",
				"target": "B",
				"Threshold": 1,
				"Label": "ActivatingOnly",
				"id": "e0"
			}
		},
		{
			"data": {
				"source": "B",
				"target": "B",
				"Threshold": 2,
				"Label": "Free",
				"id": "e1"
			}
		}
	],
	"nodes": [
		{
			"data": {
				"id": "A",
				"MaxActivity": 1
			},
			"x": 1,
			"y": 100
		},
		{
			"data": {
				"id": "B",
				"MaxActivity": 2,
				"Constraints": [
					"B:2<2"
				]
			}
		}
	]
};