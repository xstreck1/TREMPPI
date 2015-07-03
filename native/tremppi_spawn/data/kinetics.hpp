#pragma once

#include <tremppi_common/network/definitions.hpp>
#include <tremppi_common/general/common_functions.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Kinetic parameters and the corresponding parametrizations are stored in this structure.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Kinetics {
	NO_COPY(Kinetics);

    struct Param {
		string context; ///< String representation of the context.
		Levels targets; ///< Towards which level this context may regulate.
		map<CompID, Levels> requirements; ///< Levels of the source components this param is relevant to, the levels are sorted.
	};
	using Params = vector < Param > ;

    struct Component {
		CompID ID; ///< ID of the component, shared with the model
		Params params; ///< Vector of parameters, sorted lexicographically by the context.
		Configurations subcolors; ///< Subparametrizations of this particular component
	};

	vector<Component> components; ///< Species shared with the model, sorted lexicographically. 
};
