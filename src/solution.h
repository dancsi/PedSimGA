#pragma once

#include <random>
#include <cstring>

#include "vec.h"
#include "data.h"

//#define POLY_SOLUTION

#ifdef POLY_SOLUTION
const int n_points = 12;
#else
const int grid_size = 8;
#endif

struct solution_t
{
	float fitness;
#ifdef POLY_SOLUTION
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
			float angle = j * 2 * M_PI / n_points;
			//float r = j*1.5707963;
			sol.points[j] = (vec_t) data::allowed_region + r * vec_t{ sin(angle), cos(angle) };
		}
		return sol;
	}
	void mutate()
	{
		static std::mt19937 eng(123);
		std::normal_distribution<float> dist(1, 0.15);
		for (int i = 0; i < n_points; i++)
		{
			points[i] = (points[i] - data::allowed_region) * dist(eng) + data::allowed_region;
		}
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
#else
	bool grid[grid_size][grid_size];
	bool edge_field[grid_size][grid_size];
	std::mt19937 eng;

	solution_t()
	{
		memset(grid, 0, sizeof(grid));
		eng = std::mt19937(123);
	}

	bool visited[grid_size][grid_size];

	void dfs(short i, short j)
	{
		if (i < 0 || i >= grid_size || j < 0 || j >= grid_size)
		{
			return;
		}
		if (visited[i][j]) return;
		visited[i][j] = true;
		if (grid[i][j])
		{
			edge_field[i][j] = true;
			return;
		}
		dfs(i - 1, j);
		dfs(i, j - 1);
		dfs(i + 1, j);
		dfs(i, j + 1);
	}
	void find_edge_fields()
	{
		memset(visited, 0, sizeof(visited));
		memset(edge_field, 0, sizeof(edge_field));
		for (int i = 0; i < grid_size; i++)
		{
			std::pair<int, int> special_points[] = { std::make_pair(0, i ), std::make_pair( grid_size - 1, i ), std::make_pair( i, 0 ), std::make_pair( i, grid_size - 1 ) };
			for (int k = 0; k < 4; k++)
			{
				auto pt = special_points[k];
				if (grid[pt.first][pt.second])
				{
					visited[pt.first][pt.second] = true;
					edge_field[pt.first][pt.second] = true;
				}
				else
				{
					dfs(pt.first, pt.second);
				}
			}
		}
	}
	static solution_t random()
	{
		static std::mt19937 eng(123);
		std::bernoulli_distribution dist(.5);

		solution_t sol;

		for (int i = 0; i < grid_size; i++)
		{
			for (int j = 0; j < grid_size; j++)
			{
				sol.grid[i][j] = dist(eng);
			}
		}
		return sol;
	}
	void mutate()
	{
		const float mutation_percentage = 0.5;
		std::bernoulli_distribution dist(mutation_percentage);

		find_edge_fields();

		for (int i = 0; i < grid_size; i++)
		{
			for (int j = 0; j < grid_size; j++)
			{
				if (edge_field[i][j] && dist(eng))
				{
					grid[i][j] = !grid[i][j];
				}
			}
		}
	}
	solution_t crossover(solution_t& other)
	{
		std::bernoulli_distribution dist(other.fitness / (fitness + other.fitness));

		solution_t sol;

		for (int i = 0; i < grid_size; i++)
		{
			for (int j = 0; j < grid_size; j++)
			{
				if (grid[i][j] == other.grid[i][j])
				{
					sol.grid[i][j] = grid[i][j];
				}
				else
				{
					if (dist(eng))
					{
						sol.grid[i][j] = grid[i][j];
					}
					else
					{
						sol.grid[i][j] = other.grid[i][j];
					}
				}
			}
		}
		return sol;
	}
	void write(FILE* file)
	{
		find_edge_fields();
		vec_t origin = data::allowed_region - data::allowed_region.r * vec_t{ 1, 1 };
		float square_side = 2 * data::allowed_region.r / grid_size;
		for (int i = 0; i < grid_size; i++)
		{
			for (int j = 0; j < grid_size; j++)
			{
				if (edge_field[i][j])
				{
					const vec_t points[4] = {
						origin + vec_t{ i*square_side, j*square_side },
						origin + vec_t{ (i + 1)*square_side, j*square_side },
						origin + vec_t{ (i + 1)*square_side, (j + 1)*square_side },
						origin + vec_t{ i*square_side, (j + 1)*square_side },
					};
					for (int k = 0; k < 4; k++)
					{
						int kk = (k + 1) % 4;
						fprintf(file, "%f %f %f %f\n", points[k].x, points[k].y, points[kk].x, points[kk].y);
					}
				}
			}
		}
	}
#endif
	const bool operator<(const solution_t& rhs) const
	{
		return fitness < rhs.fitness;
	}
};