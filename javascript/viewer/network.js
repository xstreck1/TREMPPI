var elements = {
    "name": "test",
    "edges": [
        {
            "data": {
                "source": "n0",
                "target": "n1",
                "Threshold": 1,
                "Label": "tt",
                "id": "e0"
            }
        },
        {
            "data": {
                "source": "n1",
                "target": "n1",
                "Threshold": 2,
                "Label": "+",
                "id": "e1"
            }
        }
    ],
    "nodes": [
        {
            "data": {
                "Name": "A",
                "MaxActivity": 1,
                "id": "n0"
            },
            "position": {
                "x": 284,
                "y": 205
            }
        },
        {
            "data": {
                "Name": "C",
                "MaxActivity": 2,
                "Constraint": "B:2<2",
                "id": "n1"
            },
            "position": {
                "x": 542,
                "y": 214
            }
        }
    ]
};