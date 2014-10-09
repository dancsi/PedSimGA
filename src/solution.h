#pragma once

#include "vec.h"

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
	const bool operator<(const solution_t& rhs) const
	{
		return fitness < rhs.fitness;
	}
};