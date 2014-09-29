#include "population.h"
#include "data.h"
#include "evaluator.h"

vector<solution_t> population::population;
size_t population::size;

void population::generate_initial()
{

}

void population::evaluate()
{
	evaluator::prepare_step();
	evaluator::open_pipes();
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
