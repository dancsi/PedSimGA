#include <random>

#include "population.h"
#include "data.h"
#include "evaluator.h"

vector<solution_t> population::population;
size_t population::size;

void population::generate_initial()
{
	std::mt19937 eng;
	std::uniform_real_distribution<float> dist(0, data::allowed_region.r);
	for (int i = 0; i < population::size; i++)
	{
		solution_t sol;
		for (int j = 0; j < n_points; j++)
		{
			float r = dist(eng);
			sol.points[j] = (vec_t) data::allowed_region + vec_t{cos(r), sin(r)};
		}
		population.push_back(sol);
	}
}

void population::evaluate()
{
	evaluator::prepare_step();
	evaluator::open_pipes();
	evaluator::read_results();
	sort(population.begin(), population.end());
}

void population::crossover()
{

}

void population::mutate()
{

}

bool population::converged()
{
	return true;
}

void population::select()
{

}
