#pragma once

#include <vector>

#include "solution.h"

using namespace std;

namespace population
{
	extern vector<solution_t> population;
	extern size_t size;

	void generate_initial();
	void evaluate();
	void crossover();
	void mutate();
	bool converged();
}