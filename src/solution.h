#pragma once

#include <random>

#include "vec.h"
#include "data.h"

const int n_points = 12;

struct solution_t
{
	float fitness;
	vec_t points[n_points];
	void write(FILE* file)
	{
		for (int i = 0; i < n_points; i++)
		{
			int j = (i + 1) % n_points;
			fprintf(file, "%f %f %f %f\n", points[i].x, points[i].y, points[j].x, points[j].y);
		}
	}
	solution_t()  {}
	static solution_t random()
	{
		static std::mt19937 eng(123);
		std::uniform_real_distribution<float> dist(0, data::allowed_region.r);
		solution_t sol;
		for (int j = 0; j < n_points; j++)
		{
			float r = dist(eng);
			sol.points[j] = (vec_t) data::allowed_region + vec_t{ cos(r), sin(r) };
		}
		return sol;
	}
	solution_t crossover(solution_t& other)
	{
		static std::mt19937 eng;
		static std::uniform_int_distribution<int> dist(0, n_points-1);
		solution_t sol;
		int start = dist(eng), howmany = dist(eng);
		for (int i = 0; i < n_points; i++)
		{
			bool copy_from_this = (i >= start) && ((i<start + howmany) || (i<(start + howmany)%n_points));
			if (copy_from_this) sol.points[i] = points[i];
			else sol.points[i] = other.points[i];
		}
		return sol;
	}
	const bool operator<(const solution_t& rhs) const
	{
		return fitness < rhs.fitness;
	}
};