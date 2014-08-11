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
		CompID source; ///< Index of the regulator in the list of species
	};
	typedef vector<Regulation> Regulations;

	/// Structure that holds data about a single specie. Most of the data is equal to that in the model file.
	struct ModelSpecie {
		CompID id; ///< Numerical ID of the component
		string name; ///< Actuall name of the specie.
		ActLevel max_activity; ///< Maximal activation level of the specie.

		Regulations regulations; ///< Regulations of the specie (activations or inhibitions by other species).
	};

	vector<ModelSpecie> species; ///< vector of all species of the model
};
