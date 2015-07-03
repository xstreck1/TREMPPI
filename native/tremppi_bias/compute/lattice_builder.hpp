#pragma once

#include <tremppi_common/network/definitions.hpp>

#include "../data/data_storage.hpp"

// TODO: does not work for multi-valued
namespace LatticeBuilder
{
	// Create the lattice - for each parametrization have indices of all the parametrizations with Hamming distance 1
	NeighLattice buildLattice(const Configurations & params)
	{
		const size_t DIMS = params.front().size();
		NeighLattice lattice(params.size(), Neighbours(DIMS, INF));

		for (const ParamNo param_no : cscope(params))
		{
			Levels compare = params[param_no];
			for (const size_t dim : crange(DIMS))
			{
				compare[dim] = !compare[dim];
				StateID state = distance(begin(params), find(WHOLE(params), compare));
				lattice[param_no][dim] = state == params.size() ? INF : state;
				compare[dim] = !compare[dim];
			}
		}

		return lattice;
	}

	// How many neighbours a parametrization has in the lattice
	vector<ParamNo> getNeighbourCount(const NeighLattice & lattice)
	{
		vector<ParamNo> result(lattice.size(), 0);

		for (const ParamNo param_no : cscope(lattice)) 
			result[param_no] = lattice[param_no].size() - count(WHOLE(lattice[param_no]), INF);

		return result;
	}

	// Recursive component search
	void DFS(const NeighLattice & lattice, const size_t curr_node, set<ParamNo> & remaining_nodes, vector<ParamNo> & component)
	{
		remaining_nodes.erase(curr_node);
		component.push_back(curr_node);
		for (const ParamNo dim : cscope(lattice.front()))
			if (remaining_nodes.find(lattice[curr_node][dim]) != end(remaining_nodes))
				DFS(lattice, lattice[curr_node][dim], remaining_nodes, component);
	}

	// Obtain the components of the lattice - a vector which holds vectors of ideces of parametrizations that form a component
	vector<vector<ParamNo> > computeComponents(const NeighLattice & lattice)
	{
		vector<vector<ParamNo> > components;

		auto node_scope = cscope(lattice);
		set<ParamNo> remaining_nodes(WHOLE(node_scope));
		while (!remaining_nodes.empty())
		{
			components.push_back({});
			DFS(lattice, *remaining_nodes.begin(), remaining_nodes, components.back());
		}

		return components;
	}

	// Convert the representation from sets to labels
	vector<ParamNo> assignMembership(const size_t param_count, const vector<vector<ParamNo> > & components)
	{
		vector<ParamNo> membership(param_count, numeric_limits<ParamNo>::max()); // Size of the vector is given by size of all the components

		for (const ParamNo comp_no : cscope(components))
			for (const ParamNo & param_no : components[comp_no])
				membership[param_no] = comp_no;

		return membership;
	}


	// Build a lattice based on parametrizations
/*	void makeParLatt(Par & par)
{
		par.neigh_lattice = buildLattice(par.params);
		par.neigh_count = getNeighbourCount(par.neigh_lattice);
		par.components = computeComponents(par.neigh_lattice);
		par.component_membership = assignMembership(par.params.size(), par.components);
	}*/
}