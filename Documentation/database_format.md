TREMPPI DATABASE FORMAT
=======================

This document contains the basic information about the data storage used by TREMPPI.

All the data for a single model are stored in a single SQLITE database.

TABLE Components
----------------
This table stores components (nodes) of the regulatory graph

1. `Name TEXT` - a name of the component. **Required:** must contain only numbers, letters and underscore and must start with a letter
2. `MaxActivity INTEGER` - a maximal activity level of the component in the row. **Required:** must be in the range [1,9]

The table is required to be in a lexicographic ascending ordered by `Name`.

TABLE Regulations
-----------------
This table stores regulations (edges) of the regulatory graph

1. `Target TEXT` - a name of a target of a regulation
2. `Source TEXT` - a name of a source of a regulation
3. `Threshold INTEGER` - a threshold value. **Required:** must be in the range  [1, `MaxActivity` of the `Regulator`]

The table is required to by in a lexicographic ascending ordered by `Target`, then by `Regulator`, then by `Threshold`.

TABLE Parametrizations
----------------------
The table that stores all the results. 

1. `ID INTEGER` - a unique ID of a parametrization. **Required:** must be equal to the number of the row, counting from 1.
2. `K_$(comp_name)_$(source_configuration) INTEGER` - a column must exist for each target and each configuration of the thresholds of its regulators where `comp_name` is the name of the target and source_configuration are for each of the regulators, ordered lexicographically, the threshold values or zero. Example `K_Raf_1` for Raf with a single regulator on threshold 1 or  `K_p_53_201` for p_53 with three regulators on thresholds 2, 0 and 1 in with ordering ascending by their names. **Required:** the value must be [0, `MaxActivity` of the `comp_name`].

This is usually the only table that's being extended by adding new columns.