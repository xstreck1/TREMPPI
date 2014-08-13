#pragma once

#include <tremppi_common/network/model.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Kinetic parameters and the corresponding parametrizations are stored in this structure.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Kinetics {
	NO_COPY(Kinetics)

	struct Param {
		string context; ///< String representation of the context.
		Levels targets; ///< Towards which level this context may regulate.
		map<CompID, Levels> requirements; ///< Levels of the source components this param is relevant to, the levels are sorted.
	
		Levels target_in_subcolor; ///< List of values from different subparametrizations for this specie, share indices between params.
	};
	using Params = vector<Param>;

	struct Component {
		string name; ///< Name of the specie, shared with specie in model.
		Params params; ///< Vector of parameters, sorted lexicographically by the context.
		ParamNo col_count; ///< Number of subcolors for this specie.
		ParamNo step_size; ///< In the context of the whole parametrization space, how may changes occur between a subcolor of this specie changes?
	};

	vector<Component> components; ///< Species shared with the model, sorted lexicographically. 
};