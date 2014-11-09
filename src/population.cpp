#include <random>

#include "population.h"
#include "data.h"
#include "evaluator.h"

vector<solution_t> population::population, population::new_population;
size_t population::size;
size_t population::last_feasible_index;
int population::n_generations;

void population::generate_initial()
{
	for (int i = 0; i < population::size; i++)
	{
		population.push_back(solution_t::random());
	}
}

void population::evaluate()
{
	n_generations++;
	evaluator::prepare_step();
	evaluator::run();
	sort(population.begin(), population.end());
}



void population::crossover()
{
	vector<float> selection_weights;
	float best_fitness = population[0].fitness, sum = 0;
	for (int i = 0; i < last_feasible_index; i++)
	{
		selection_weights.push_back(best_fitness / population[i].fitness);
	}

	weighted_random::reset(selection_weights);

	int n_offsprings = last_feasible_index / 2;
	for (int i = 0; i < n_offsprings; i++)
	{
		int a = weighted_random::select(), b = weighted_random::select();
		new_population.push_back(population[a].crossover(population[b]));
	}
	for (int i = 0; i < n_offsprings; i++)
	{
		new_population.push_back(population[i]);
	}
}

void population::mutate()
{
	while (new_population.size() < population::size)
	{
		new_population.push_back(population[weighted_random::select()]);
		new_population.rbegin()->mutate();
	}
}

bool population::converged()
{
	return false;
}

void population::select()
{
	float best_fitness = population[0].fitness;
	last_feasible_index = 0;
	for (int i = 0; i < population::size && (population[i].fitness / best_fitness < 5); i++, last_feasible_index++);
	new_population.clear();
}

void population::show_statistics()
{
	fprintf(stderr, "***GENERATION %4d***\n", n_generations);
	fprintf(stderr, "Best: %4.4f, Worst %4.4f\n", population[0].fitness, population[last_feasible_index - 1].fitness);

}

std::discrete_distribution<> population::weighted_random::dist;
std::mt19937 population::weighted_random::generator;

void population::weighted_random::reset(std::vector<float>& weights)
{
	size_t i = 0;
	dist = std::discrete_distribution<>(weights.size(),
		weights.front(), weights.back(), [&weights, &i](double){return weights[i++]; });

}

int population::weighted_random::select()
{
	return dist(generator);
}
