#include <iostream>
#include <cstdio>
#include <vector>

#include "data.h"
#include "evaluator.h"
#include "geometry.h"
#include "population.h"
#include "solution.h"

using namespace std;

void load_data()
{
	population::size = 10;
	FILE* f_walls = fopen("walls.dat", "r");
	if (!f_walls) return;
	data::read_walls(f_walls);
	fclose(f_walls);
}

int main()
{
	load_data();
	evaluator::init();

	data::allowed_region = {38, 20, 5};

	population::generate_initial();
	do 
	{
		population::evaluate();
		population::select();
		population::crossover();
		population::mutate();
		population::population.swap(population::new_population);
	} while (!population::converged());

	return 0;
}