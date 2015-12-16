/******************************************************************************
Created by Adam Streck, 2013-2015, adam.streck@fu-berlin.de

This file is part of the Toolkit for Reverse Engineering of Molecular Pathways
via Parameter Identification (TREMPPI)

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/
#pragma once

#include "model.hpp"


namespace ModelTranslators 
{
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
}