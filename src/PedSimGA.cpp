#include <iostream>
#include <cstdio>
#include <vector>
#include <chrono>

#include "data.h"
#include "evaluator.h"
#include "geometry.h"
#include "population.h"
#include "solution.h"

using namespace std;

void load_data()
{
	FILE* f_walls = fopen("walls.dat", "r");
	if (!f_walls) return;
	data::read_walls(f_walls);
	fclose(f_walls);
}

int main()
{
	fprintf(stderr, "Population size?\n");
	scanf("%u", &population::size);
	int total_generations;
	fprintf(stderr, "How many generations?\n");
	scanf("%d", &total_generations);

	load_data();
	evaluator::init();

	data::allowed_region = {37, 20, 1.5};

	population::generate_initial();
	do 
	{
		auto start_time = chrono::high_resolution_clock::now();
		population::evaluate();
		population::select();
		population::crossover();
		population::mutate();
		auto end_time = chrono::high_resolution_clock::now();
		population::show_statistics();
		fprintf(stderr, "Wall time elapsed: %.3lfs\n", chrono::duration<double>(end_time-start_time).count());
		if (population::n_generations == total_generations)
		{
			fprintf(stderr, "How many additional generations?\n");
			int additional;
			scanf("%d", &additional);
			total_generations += additional;
			if (!additional)
			{
				evaluator::finish();
				break;
			}
		}
		
		population::population.swap(population::new_population);
		
	} while (!population::converged());

	return 0;
}