#pragma once

#include "../data/data.hpp"

namespace MVQMC
{
	/* Return a vector of combined values, if possible, otherwise return an empty vector. */
	PMin combine(PMin & a, PMin & b);

	/* Return a vector of combinations of the next order. Those who were not possible to merge are left in the reference to input. */
	vector<PMin> next_merge(vector<PMin> & original);

	vector<PMin> valuesToCompat(vector<vector<int> > original);

	/* Create representation of the vector with don't cares. */
	vector<PMin> compactize(vector<PMin> current);
}