tremppi_report.setup = {
   "comparative" : true,
   "compare" : "(NOT (Rosen AND Rosen AND RosenB AND Triv_Fixpoint AND CycAttractor_MAPK AND CycAttractor_mTor AND  Mut_FP))",
   "compared" : 12986,
   "date" : "Thu Jul 03 12:07:36 2014",
   "name" : "crossfunc",
   "pool_size" : 13266,
   "select" : "(Rosen AND Rosen AND RosenB AND Triv_Fixpoint AND CycAttractor_MAPK AND CycAttractor_mTor AND  Mut_FP)",
   "selected" : 280
};
tremppi_report.Regulatory.select = {
   "elements" : {
      "edges" : [
         {
            "data" : {
               "frequency" : 1.0,
               "pearson" : 1.0,
               "source" : "PI3K",
               "target" : "Akt"
            }
         },
         {
            "data" : {
               "frequency" : 1.0,
               "pearson" : 1.0,
               "source" : "Mek",
               "target" : "Erk"
            }
         },
         {
            "data" : {
               "frequency" : 1.0,
               "pearson" : 1.0,
               "source" : "Raf",
               "target" : "Mek"
            }
         },
         {
            "data" : {
               "frequency" : 0.8181818181818182,
               "pearson" : -0.3489016221354863,
               "source" : "Erk",
               "target" : "PI3K"
            }
         },
         {
            "data" : {
               "frequency" : 1.0,
               "pearson" : 0.4538743983518276,
               "source" : "RTK",
               "target" : "PI3K"
            }
         },
         {
            "data" : {
               "frequency" : 1.0,
               "pearson" : -0.4538743983518208,
               "source" : "mTor",
               "target" : "PI3K"
            }
         },
         {
            "data" : {
               "frequency" : 1.0,
               "pearson" : 1.0,
               "source" : "RTK",
               "target" : "RTK"
            }
         },
         {
            "data" : {
               "frequency" : 0.9268292682926830,
               "pearson" : -0.3442451657755377,
               "source" : "Akt",
               "target" : "Raf"
            }
         },
         {
            "data" : {
               "frequency" : 1.0,
               "pearson" : -0.3796634992642713,
               "source" : "Erk",
               "target" : "Raf"
            }
         },
         {
            "data" : {
               "frequency" : 0.9105691056910569,
               "pearson" : 0.3102120148567476,
               "source" : "PI3K",
               "target" : "Raf"
            }
         },
         {
            "data" : {
               "frequency" : 1.0,
               "pearson" : 0.3796634992642704,
               "source" : "RTK",
               "target" : "Raf"
            }
         },
         {
            "data" : {
               "frequency" : 1.0,
               "pearson" : -0.7182335127931944,
               "source" : "Akt",
               "target" : "Tsc"
            }
         },
         {
            "data" : {
               "frequency" : 0.6666666666666666,
               "pearson" : -0.3849001794597454,
               "source" : "Erk",
               "target" : "Tsc"
            }
         },
         {
            "data" : {
               "frequency" : 0.6666666666666666,
               "pearson" : 0.3849001794597454,
               "source" : "Erk",
               "target" : "mTor"
            }
         },
         {
            "data" : {
               "frequency" : 1.0,
               "pearson" : -0.7182335127931944,
               "source" : "Tsc",
               "target" : "mTor"
            }
         }
      ],
      "nodes" : [
         {
            "data" : {
               "id" : "Akt",
               "name" : "Akt"
            }
         },
         {
            "data" : {
               "id" : "Erk",
               "name" : "Erk"
            }
         },
         {
            "data" : {
               "id" : "Mek",
               "name" : "Mek"
            }
         },
         {
            "data" : {
               "id" : "PI3K",
               "name" : "PI3K"
            }
         },
         {
            "data" : {
               "id" : "RTK",
               "name" : "RTK"
            }
         },
         {
            "data" : {
               "id" : "Raf",
               "name" : "Raf"
            }
         },
         {
            "data" : {
               "id" : "Tsc",
               "name" : "Tsc"
            }
         },
         {
            "data" : {
               "id" : "mTor",
               "name" : "mTor"
            }
         }
      ]
   }
};
tremppi_report.Regulatory.differ = {
   "elements" : {
      "edges" : [
         {
            "data" : {
               "frequency" : 0.0,
               "pearson" : 0.0,
               "source" : "PI3K",
               "target" : "Akt"
            }
         },
         {
            "data" : {
               "frequency" : 0.0,
               "pearson" : 0.0,
               "source" : "Mek",
               "target" : "Erk"
            }
         },
         {
            "data" : {
               "frequency" : 0.0,
               "pearson" : 0.0,
               "source" : "Raf",
               "target" : "Mek"
            }
         },
         {
            "data" : {
               "frequency" : 0.0,
               "pearson" : -1.221245327087672e-015,
               "source" : "Erk",
               "target" : "PI3K"
            }
         },
         {
            "data" : {
               "frequency" : 0.0,
               "pearson" : 1.493249968120836e-014,
               "source" : "RTK",
               "target" : "PI3K"
            }
         },
         {
            "data" : {
               "frequency" : 0.0,
               "pearson" : -1.487698852997710e-014,
               "source" : "mTor",
               "target" : "PI3K"
            }
         },
         {
            "data" : {
               "frequency" : 0.0,
               "pearson" : 0.0,
               "source" : "RTK",
               "target" : "RTK"
            }
         },
         {
            "data" : {
               "frequency" : -0.008918820531488958,
               "pearson" : 0.01223129858231259,
               "source" : "Akt",
               "target" : "Raf"
            }
         },
         {
            "data" : {
               "frequency" : 0.0,
               "pearson" : 0.01039128593768190,
               "source" : "Erk",
               "target" : "Raf"
            }
         },
         {
            "data" : {
               "frequency" : 0.007341342070137147,
               "pearson" : 0.02180185233647791,
               "source" : "PI3K",
               "target" : "Raf"
            }
         },
         {
            "data" : {
               "frequency" : 0.0,
               "pearson" : -0.01039128593768085,
               "source" : "RTK",
               "target" : "Raf"
            }
         },
         {
            "data" : {
               "frequency" : 0.0,
               "pearson" : -1.099120794378905e-014,
               "source" : "Akt",
               "target" : "Tsc"
            }
         },
         {
            "data" : {
               "frequency" : 0.0,
               "pearson" : -2.048361480433414e-014,
               "source" : "Erk",
               "target" : "Tsc"
            }
         },
         {
            "data" : {
               "frequency" : 0.0,
               "pearson" : 2.048361480433414e-014,
               "source" : "Erk",
               "target" : "mTor"
            }
         },
         {
            "data" : {
               "frequency" : 0.0,
               "pearson" : -1.099120794378905e-014,
               "source" : "Tsc",
               "target" : "mTor"
            }
         }
      ],
      "nodes" : [
         {
            "data" : {
               "id" : "Akt",
               "name" : "Akt"
            }
         },
         {
            "data" : {
               "id" : "Erk",
               "name" : "Erk"
            }
         },
         {
            "data" : {
               "id" : "Mek",
               "name" : "Mek"
            }
         },
         {
            "data" : {
               "id" : "PI3K",
               "name" : "PI3K"
            }
         },
         {
            "data" : {
               "id" : "RTK",
               "name" : "RTK"
            }
         },
         {
            "data" : {
               "id" : "Raf",
               "name" : "Raf"
            }
         },
         {
            "data" : {
               "id" : "Tsc",
               "name" : "Tsc"
            }
         },
         {
            "data" : {
               "id" : "mTor",
               "name" : "mTor"
            }
         }
      ]
   }
};
tremppi_report.Regulatory.compare = {
   "elements" : {
      "edges" : [
         {
            "data" : {
               "frequency" : 1.0,
               "pearson" : 1.0,
               "source" : "PI3K",
               "target" : "Akt"
            }
         },
         {
            "data" : {
               "frequency" : 1.0,
               "pearson" : 1.0,
               "source" : "Mek",
               "target" : "Erk"
            }
         },
         {
            "data" : {
               "frequency" : 1.0,
               "pearson" : 1.0,
               "source" : "Raf",
               "target" : "Mek"
            }
         },
         {
            "data" : {
               "frequency" : 0.8181818181818182,
               "pearson" : -0.3489016221354863,
               "source" : "Erk",
               "target" : "PI3K"
            }
         },
         {
            "data" : {
               "frequency" : 1.0,
               "pearson" : 0.4538743983518276,
               "source" : "RTK",
               "target" : "PI3K"
            }
         },
         {
            "data" : {
               "frequency" : 1.0,
               "pearson" : -0.4538743983518208,
               "source" : "mTor",
               "target" : "PI3K"
            }
         },
         {
            "data" : {
               "frequency" : 1.0,
               "pearson" : 1.0,
               "source" : "RTK",
               "target" : "RTK"
            }
         },
         {
            "data" : {
               "frequency" : 0.9268292682926830,
               "pearson" : -0.3442451657755377,
               "source" : "Akt",
               "target" : "Raf"
            }
         },
         {
            "data" : {
               "frequency" : 1.0,
               "pearson" : -0.3796634992642713,
               "source" : "Erk",
               "target" : "Raf"
            }
         },
         {
            "data" : {
               "frequency" : 0.9105691056910569,
               "pearson" : 0.3102120148567476,
               "source" : "PI3K",
               "target" : "Raf"
            }
         },
         {
            "data" : {
               "frequency" : 1.0,
               "pearson" : 0.3796634992642704,
               "source" : "RTK",
               "target" : "Raf"
            }
         },
         {
            "data" : {
               "frequency" : 1.0,
               "pearson" : -0.7182335127931944,
               "source" : "Akt",
               "target" : "Tsc"
            }
         },
         {
            "data" : {
               "frequency" : 0.6666666666666666,
               "pearson" : -0.3849001794597454,
               "source" : "Erk",
               "target" : "Tsc"
            }
         },
         {
            "data" : {
               "frequency" : 0.6666666666666666,
               "pearson" : 0.3849001794597454,
               "source" : "Erk",
               "target" : "mTor"
            }
         },
         {
            "data" : {
               "frequency" : 1.0,
               "pearson" : -0.7182335127931944,
               "source" : "Tsc",
               "target" : "mTor"
            }
         }
      ],
      "nodes" : [
         {
            "data" : {
               "id" : "Akt",
               "name" : "Akt"
            }
         },
         {
            "data" : {
               "id" : "Erk",
               "name" : "Erk"
            }
         },
         {
            "data" : {
               "id" : "Mek",
               "name" : "Mek"
            }
         },
         {
            "data" : {
               "id" : "PI3K",
               "name" : "PI3K"
            }
         },
         {
            "data" : {
               "id" : "RTK",
               "name" : "RTK"
            }
         },
         {
            "data" : {
               "id" : "Raf",
               "name" : "Raf"
            }
         },
         {
            "data" : {
               "id" : "Tsc",
               "name" : "Tsc"
            }
         },
         {
            "data" : {
               "id" : "mTor",
               "name" : "mTor"
            }
         }
      ]
   }
};
