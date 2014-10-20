#include <random>

#include "population.h"
#include "data.h"
#include "evaluator.h"

vector<solution_t> population::population, population::new_population;
size_t population::size;
size_t population::last_feasible_index;

void population::generate_initial()
{
	for (int i = 0; i < population::size; i++)
	{
		population.push_back(solution_t::random());
	}
}

void population::evaluate()
{
	evaluator::prepare_step();
	evaluator::open_pipes();
	evaluator::read_results();
	sort(population.begin(), population.end());
}

int weighted_random_select(vector<float>& weights)
{
	static std::mt19937 eng;
	static std::uniform_real_distribution<float> dist(0, 1);
	float r = dist(eng);
	int ret;
	for (ret = 0; ret < weights.size() && r>weights[ret]; r -= weights[ret], ret++);
	return ret;
}

void population::crossover()
{
	vector<float> weights;
	float best_fitness = population[0].fitness, sum = 0;
	for (int i = 0; i < last_feasible_index; i++)
	{
		weights.push_back(best_fitness/population[i].fitness);
		sum += weights[i];
	}
	for (float& weight : weights) weight /= sum;
	int n_offsprings = last_feasible_index / 2;
	for (int i = 0; i < n_offsprings; i++)
	{
		int a = weighted_random_select(weights), b = weighted_random_select(weights);
		new_population.push_back(population[a].crossover(population[b]));
	}
	for (int i = 0; i < n_offsprings; i++)
	{
		new_population.push_back(population[i]);
	}
}

void population::mutate()
{
	while (new_population.size()<population::size)
	{
		new_population.push_back(solution_t::random());
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
	for (int i = 0; i < population::size && (population[i].fitness/best_fitness < 2.5); i++, last_feasible_index++);
	new_population.clear();
}
