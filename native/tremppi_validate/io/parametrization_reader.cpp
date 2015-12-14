#include "parametrization_reader.hpp"

ParametrizationReader::ParametrizationReader(const RegInfos & reg_infos) {
	for (const RegInfo & reg_info : reg_infos) 
	{
		// Only components that are multi-valued and self-regulating can be not normalized and require the check
		if (reg_info.regulators.count(reg_info.ID) && reg_info.max_activity > 1)
		{
			components_to_check.push_back(reg_info.ID);
		}
	}
	if (!components_to_check.empty()) 
	{
		for (const CompID ID : components_to_check) 
		{
			for (const pair<size_t, Configurations> & requirement : reg_infos[ID].requirements) 
			{
				neighbour_contexts[make_pair(requirement.first, true)] = find_neighbour(requirement.second, true, reg_infos[ID]);
				neighbour_contexts[make_pair(requirement.first, false)] = find_neighbour(requirement.second, false, reg_infos[ID]);
			}
		}
	}
}

void ParametrizationReader::select(const RegInfos & reg_infos, const string & selection, sqlite3pp::database & db) 
{
	vector<string> contexts = DataInfo::getAllContexts(reg_infos);
	if (contexts.empty()) 
	{
		throw runtime_error("No parameter columns found in the database.");
	}

	const string qry_string = "SELECT ROWID, " + boost::algorithm::join(contexts, ", ") + " FROM " + PARAMETRIZATIONS_TABLE + " WHERE " + selection;
	selection_qry.reset(new sqlite3pp::query(db, (qry_string).c_str()));
	sel_it = selection_qry->begin();
}

const Levels & ParametrizationReader::getParametrization() const 
{
	return current_parametrization;
}

// Iterate, return false if the query came to an end
bool ParametrizationReader::next() 
{
	if (sel_it == selection_qry->end())
	{
		return false;
	}

	current_rowid = sel_it->get<ParamNo>(0);
	current_parametrization = sqlite3pp::func::getRow<ActLevel>(*sel_it, 1, selection_qry->column_count() - 1);
	sel_it++;
	return true;
}

ActLevel ParametrizationReader::normalize(const Levels & original, const RegInfo & reg_info, const size_t column, Levels & normalized)
{
	const Levels& act_int = reg_info.requirements.at(column).at(reg_info.ID);
	const ActLevel par_val = original.at(column);

	if (par_val < act_int.front()) 
	{
		size_t new_column = neighbour_contexts.at(make_pair(column, false));
		const ActLevel new_par = original.at(new_column);
		if (reg_info.requirements.at(new_column).at(reg_info.ID).back() <= new_par)
		{
			normalized[column] = reg_info.requirements.at(new_column).at(reg_info.ID).back();
		}
		else
		{
			normalized[column] = normalize(original, reg_info, new_column, normalized);
		}
	}
	else if (par_val > act_int.back())
	{
		size_t new_column = neighbour_contexts.at(make_pair(column, true));
		const ActLevel new_par = original.at(new_column);
		if (reg_info.requirements.at(new_column).at(reg_info.ID).front() >= new_par)
		{
			normalized[column] = reg_info.requirements.at(new_column).at(reg_info.ID).front();
		}
		else
		{
			normalized[column] = normalize(original, reg_info, new_column, normalized);
		}
	}
	else
	{
		normalized[column] = par_val;
	}
	return normalized[column];
}

bool ParametrizationReader::isNormalized(const Levels & parametrization, const RegInfos & reg_infos)
{
	Levels normalized(parametrization.size(), numeric_limits<ActLevel>::max());
	for (const CompID ID : components_to_check) 
	{
		for (const pair<size_t, string> & column : reg_infos[ID].columns) 
		{
			if (normalized[column.first] == numeric_limits<ActLevel>::max())
			{
				normalize(parametrization, reg_infos[ID], column.first, normalized);
			}
		}
	}
	// for_each(WHOLE(normalized), [](ActLevel act) {cout << act << ",";}); // Prints normalized
	return equal(WHOLE(parametrization), begin(normalized), [](const ActLevel A, const ActLevel B) {return B == numeric_limits<ActLevel>::max() || A == B;});
}

ParamNo ParametrizationReader::getRowID() const 
{
	return current_rowid;
}


int ParametrizationReader::find_neighbour(const Configurations & original, const bool direction, const RegInfo & reg_info)
{
	// The self-regulation activity interval is on the boundary, no neighbour exists
	if ((direction == false && original[reg_info.ID].front() == 0) ||
		(direction == true && original[reg_info.ID].back() == reg_info.max_activity))
	{
		return -1;
	}
	else 
	{
		for (const pair<size_t, Configurations> requirement : reg_info.requirements) 
		{
			for (const CompID ID : cscope(original))
			{
				if (ID == reg_info.ID)
				{
					if ((direction == false && original[ID].front() - 1 != requirement.second[ID].back()) ||
						(direction == true && original[ID].back() + 1 != requirement.second[ID].front()))
					{
						continue;
					}
				}
				else {
					if (original[ID] != requirement.second[ID])
					{
						continue;
					}
				}
				if (ID == original.size() - 1)
				{
					return requirement.first;
				}
			}
		}
	}
	throw runtime_error("The neighbour context search routine failed for the component " + reg_info.name);
}