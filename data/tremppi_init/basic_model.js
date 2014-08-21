var network = {
        "edges": [
            {
                "data": {
                    "source": "A",
                    "target": "B",
                    "Threshold": 1,
                    "Label": "ActivatingOnly"
                }
            },
            {
                "data": {
                    "source": "B",
                    "target": "B",
                    "Threshold": 2,
                    "Label": "Free"
                }
            }
        ],
        "nodes": [
            {
                "data": {
                    "id": "A",
                    "MaxActivity": 1
                }
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
