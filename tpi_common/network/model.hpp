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
      StateID source; ///< Regulator specie ID.
      ActLevel threshold; ///< Level of the regulator required for the regulation to be active.
      string name; ///< Name of the regulator.
   };
   typedef vector<Regulation> Regulations;

   /// Structure that holds data about a single specie. Most of the data is equal to that in the model file.
   struct ModelSpecie {
      string name; ///< Actuall name of the specie.
      ActLevel max_value; ///< Maximal activation level of the specie.
	  vector<string> par_cons;  ///< Constraints on the parameters.

      Regulations regulations; ///< Regulations of the specie (activations or inhibitions by other species).
   };

   vector<ModelSpecie> species; ///< vector of all species of the model
};
