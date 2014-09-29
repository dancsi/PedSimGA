#include "data.h"

std::vector<geometry::line_t> data::walls;


void data::read_walls(FILE* f_walls)
{
	geometry::line_t l;
	while (fscanf(f_walls, "%f%f%f%f", &l.p.x, &l.p.y, &l.q.x, &l.q.y) == 4)
	{
		data::walls.push_back(l);
	}
}

void data::write_walls(FILE* f_walls)
{
	for (geometry::line_t& l : walls)
	{
		fprintf(f_walls, "%f %f %f %f\n", l.p.x, l.p.y, l.q.x, l.q.y);
	}
}
