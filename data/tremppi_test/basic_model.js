var model = {
   "elements" : {
      "edges" : [
         {
            "data" : {
               "source" : "A",
               "target" : "B",
			   "Threshold" : 1,
			   "Label" : "ActivatingOnly"
            }
         },
         {
            "data" : {
               "source" : "B",
               "target" : "B",
			   "Threshold" : 2,
			   "Label" : "Free"
            }
         }
      ],
      "nodes" : [
		 {
            "data" : {
			   "Name" : "A",
			   "MaxActivity" : 1
            }
         },
         {
            "data" : {
			   "Name" : "B",
			   "MaxActivity" : 2
            }
         }
      ]
   }
}
