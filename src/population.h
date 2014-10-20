#pragma once

#include <vector>

#include "solution.h"

using namespace std;

namespace population
{
	extern vector<solution_t> population, new_population;
	extern size_t size;
	extern size_t last_feasible_index;
	extern int n_generations;

	void generate_initial();
	void evaluate();
	void select();
	void crossover();
	void mutate();
	bool converged();

	void show_statistics();
}