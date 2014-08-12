#pragma once

#include "definitions.hpp"
#include "../general/common_functions.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Storage for data parsed from the model.
///
/// Model stores model data in the raw form, almost the same as in the model file itself.
/// Model data can be set only form the ModelParser object.
/// Rest of the code can access the data only via constant getters - once the data are parse, model remains constant.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Model {
	NO_COPY(Model)

	struct Regulation {
		ActLevel threshold; ///< Level of the regulator required for the regulation to be active.
		CompID source; ///< Index of the regulator in the list of components.
		string label; ///< An edge semantics label, if provided.
	};
	typedef vector<Regulation> Regulations;

	/// Structure that holds data about a single component. Most of the data is equal to that in the model file.
	struct ModelComp {
		CompID id; ///< Numerical ID of the component.
		string name; ///< Actuall name of the specie.
		ActLevel max_activity; ///< Maximal activation level of the specie.

		Regulations regulations; ///< Regulations of the specie (activations or inhibitions by other components).

		vector<string> constraints; ///< A set of constraints on the component, if provided.
	};

	string name; ///< Model identifier.
	vector<ModelComp> components; ///< Vector of all components of the model.
};
