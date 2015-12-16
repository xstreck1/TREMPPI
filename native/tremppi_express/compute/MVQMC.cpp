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

#include "MVQMC.hpp"

PMin MVQMC::combine(PMin & a, PMin & b)
{
	PMin result;

	vector<bool> different(a.size());
	rng::transform(a, b, different.begin(), not_equal_to<vector<int> >()); // Get true if the vectors of values are equal
	const size_t diff_count = rng::count(different, true);

	if (diff_count == 0)
	{
		throw runtime_error("Trying to merge duplicated vectors.");
	}

	if (diff_count == 1)
	{
		result = a;
		size_t diff_position = distance(different.begin(), rng::find(different, true));
		rng::set_difference(b[diff_position], a[diff_position], inserter(result[diff_position], result[diff_position].end()));
		rng::sort(result[diff_position]);
	}

	return result;
}

vector<PMin> MVQMC::next_merge(vector<PMin> & original)
{
	vector<PMin> compacted;

	vector<bool> success(original.size(), false);
	for (auto it1 = original.begin(); it1 != original.end(); it1++)
	{
		for (auto it2 = it1 + 1; it2 != original.end(); it2++) {
			PMin combined = combine(*it1, *it2);

			if (!combined.empty())
			{
				if (rng::find(compacted, (combined)) == compacted.end())
					compacted.emplace_back(combined);
				success[distance(original.begin(), it1)] = true;
				success[distance(original.begin(), it2)] = true;
			}
		}
	}

	// Replace the original vector with the leftovers
	vector<PMin> leftovers;
	for (const size_t i : cscope(original))
		if (!success[i])
			leftovers.emplace_back(original[i]);
	original.resize(leftovers.size());
	copy(WHOLE(leftovers), original.begin());

	return compacted;
}
vector<PMin> MVQMC::valuesToCompat(vector<vector<int> > original)
{
	vector<PMin> result;

	vector<PMin> current(original.size());
	rng::transform(original, current.begin(), [](const vector<int> & member)
	{
		PMin result;
		for (const int i : member) result.emplace_back(PLit{ i });
		return result;
	});

	return result;
}

vector<PMin> MVQMC::compactize(vector<PMin> current)
{
	vector<PMin> result;

	// Make compact
	vector<PMin> compacted;
	do
	{
		compacted = next_merge(current);
		rng::copy(current, inserter(result, result.end()));
		current = compacted;
	} while (!compacted.empty());

	return result;
}