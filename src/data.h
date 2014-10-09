#pragma once

#include <vector>

#include "geometry.h"

namespace data
{
	extern std::vector<line_t> walls;
	extern circle_t allowed_region;
	void read_walls(FILE* f_walls);
	void write_walls(FILE* f_walls);
}