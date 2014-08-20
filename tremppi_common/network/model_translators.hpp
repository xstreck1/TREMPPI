/*
 * Copyright (C) 2012-2013 - Adam Streck
 * This file is a part of the ParSyBoNe (Parameter Synthetizer for Boolean Networks) verification tool.
 * ParSyBoNe is a free software: you can redistribute it and/or modify it under the terms of the GNU General Public License version 3.
 * ParSyBoNe is released without any warranty. See the GNU General Public License for more details. <http://www.gnu.org/licenses/>.
 * For affiliations see <http://www.mi.fu-berlin.de/en/math/groups/dibimath> and <http://sybila.fi.muni.cz/>.
 */

#pragma once

#include "definitions.hpp"
#include "model.hpp"

namespace ModelTranslators {
	/**
	 * @brief findID  obtain ID of the component.
	 */
	CompID findID(const Model & model, const string & name);

	/**
	 * @return	unique IDs of regulators of the component
	 */
	vector<CompID> getRegulatorsIDs(const Model & model, const CompID ID);

	/**
	 * @return	names of the regulators of the component
	 */
	vector<string> getRegulatorsNames(const Model & model, const CompID ID);

	/**
	* @return	names of all the components
	*/
	vector<string> getAllNames(const Model & model);

	/**
	 * @brief getThresholds Finds a list of thresholds for each regulator of a given component.
	 * @param ID
	 * @return
	 */
	map<CompID, Levels > getThresholds(const Model & model, const CompID ID);

	/**
	 * @brief getThreshold  For a given regulator, find out what it's threshold in the given context is.
	 * @return  threshold value in the given context
	 */
	ActLevel getThreshold(const Model & model, const string & context, const CompID t_ID, const string & name, const size_t pos);

	/**
	 * @brief formCanonicContext   Transforms the regulation specification into a canonic form (\forall r \in regulator [r:threshold,...]).
	 * @param context any valid context form as a string
	 * @return canonic context form
	 */
	string makeCanonic(const Model & model, const string & context, const CompID t_ID);

	// @return regulation with given parameters
	const Model::Regulation & findRegulation(const Model & model, const CompID t_ID, const CompID s_ID, const ActLevel threshold);

	// @return the maximal level in between the components
	const ActLevel getMaxLevel(const Model & model);

	// @return canonic form of the label
	string readLabel(const string & label);
}