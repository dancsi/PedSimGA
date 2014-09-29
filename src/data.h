#pragma once

#include <vector>

#include "geometry.h"

namespace data
{
	extern std::vector<geometry::line_t> walls;
	void read_walls(FILE* f_walls);
	void write_walls(FILE* f_walls);
}