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