#pragma once

#include <vector>
#include <string>

namespace evaluator
{
	void init();
	void create_directories();
	void open_pipes();
	void prepare_step();
	extern std::vector<std::string> directory_names;
}