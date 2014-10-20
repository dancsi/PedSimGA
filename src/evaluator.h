#pragma once

#include <vector>
#include <string>

namespace evaluator
{
	void init();
	void create_directories();
	void open_pipes();
	void read_results();
	void prepare_step();
	void finish();

	extern std::vector<std::string> directory_names;
	extern std::vector<FILE*> executable_pipes;
}