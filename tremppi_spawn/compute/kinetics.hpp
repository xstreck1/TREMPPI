/*
* Copyright (C) 2012-2014 - Adam Streck
* This file is a part of the ParSyBoNe (Parameter Synthetizer for Boolean Networks) verification tool.
* ParSyBoNe is a free software: you can redistribute it and/or modify it under the terms of the GNU General Public License version 3.
* ParSyBoNe is released without any warranty. See the GNU General Public License for more details. <http://www.gnu.org/licenses/>.
* For affiliations see http://www.mi.fu-berlin.de/en/math/groups/dibimath and http://sybila.fi.muni.cz/ .
*/

#pragma once

#include "../auxiliary/common_functions.hpp"
#include "../auxiliary/output_streamer.hpp"

struct Kinetics {
	struct Param {
		string context; ///< String representation of the context.
		Levels targets; ///< Towards which level this context may regulate.
		map<SpecieID, Levels> requirements; ///< Levels of the source species this param is relevant to, the levels are sorted.
	
		Levels target_in_subcolor; ///< List of values from different subparametrizations for this specie, share indices between params.
		bool functional; ///< True if the param is permitted to occur by the experiment 
	};
	using Params = vector<Param>;

	struct Specie {
		string name; ///< Name of the specie, shared with specie in model.
		Params params; ///< Vector of parameters, sorted lexicographically by the context.
		ParamNo col_count; ///< Number of subcolors for this specie.
		ParamNo step_size; ///< In the context of the whole parametrization space, how may changes occur between a subcolor of this specie changes?
	};

	vector<Specie> species; ///< Species shared with the model, sorted lexicographically. 
};