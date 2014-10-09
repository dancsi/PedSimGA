#include "data.h"

std::vector<line_t> data::walls;
circle_t data::allowed_region;

void data::read_walls(FILE* f_walls)
{
	line_t l;
	while (fscanf(f_walls, "%f%f%f%f", &l.p.x, &l.p.y, &l.q.x, &l.q.y) == 4)
	{
		data::walls.push_back(l);
	}
}

void data::write_walls(FILE* f_walls)
{
	for (line_t& l : walls)
	{
		fprintf(f_walls, "%f %f %f %f\n", l.p.x, l.p.y, l.q.x, l.q.y);
	}
}
