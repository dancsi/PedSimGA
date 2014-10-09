#include "evaluator.h"

#include <io.h>     // For access().
#include <direct.h>
#include <sys/types.h>  // For stat().
#include <sys/stat.h>   // For stat().

#include <cstring>
#include <cstdio>

#include "data.h"
#include "population.h"

using namespace std;
using namespace evaluator;

vector<string> evaluator::directory_names;
vector<FILE*> evaluator::executable_pipes;

const char* working_dir_name = "GA";
const char* executable_name = "PedestrianSimulator.exe";
const char* config_ini_name = "config.ini";

char working_dir[_MAX_PATH] = ""; //GA folder
char root_dir[_MAX_PATH]; //Tamo gde ocekujemo da se nalaze config.ini i exe
char executable_path[_MAX_PATH + 20]; //PedestrianSimulator.exe
char config_ini_path[_MAX_PATH + 20];
char executable_cmdline[2 * _MAX_PATH + 100];

void evaluator::create_directories()
{
	if (strlen(working_dir))
	{
		chdir(working_dir);
	}
	else
	{
		mkdir(working_dir_name);
		chdir(working_dir_name);
		getcwd(working_dir, _MAX_PATH);
	}

	char buf[20];
	int max_len = strlen(_itoa(population::size, buf, 10));
	char format_string[20];
	sprintf(format_string, "%%0%dd", max_len);

	for (int i = 0; i < population::size; i++)
	{
		sprintf(buf, format_string, i);
		directory_names.push_back(string(buf));
		printf("%s\n", buf);
	}

	for (string& dirname : directory_names)
	{
		mkdir(dirname.c_str());
		chdir(dirname.c_str());
		mkdir("logs");
		chdir("..");
	}
	chdir("..");
}

void evaluator::open_pipes()
{
	executable_pipes.clear();
	chdir(working_dir);
	for (int i = 0; i < population::size; i++)
	{
		chdir(directory_names[i].c_str());
		executable_pipes.push_back(_popen(executable_cmdline, "r"));
		chdir("..");
	}
}

void evaluator::init()
{
	getcwd(root_dir, _MAX_PATH);
	//da proverimo da li je sve tu gde treba da bude
	if (access(executable_name, 0) == 0)
	{
		_fullpath(executable_path, executable_name, sizeof(executable_path));
	}
	else
	{
		fprintf(stderr, "%s not found", executable_name);
		exit(1);
	}
	if (access(config_ini_name, 0) == 0)
	{
		_fullpath(config_ini_path, config_ini_name, sizeof(config_ini_path));
	}
	else
	{
		fprintf(stderr, "%s not found", config_ini_name);
		exit(1);
	}

	strcpy(executable_cmdline, executable_path);
	strcat(executable_cmdline, " ");
	strcat(executable_cmdline, config_ini_path);

	create_directories();
}

void evaluator::prepare_step()
{
	chdir(working_dir);
	for (int i = 0; i < population::size; i++)
	{
		string subdir = directory_names[i];
		chdir(subdir.c_str());
		FILE* f_walls = fopen("walls.dat", "w");
		data::write_walls(f_walls);
		population::population[i].write(f_walls);
		fclose(f_walls);
		chdir("..");
	}
}

void evaluator::read_results()
{
	for (int i = 0; i < population::size; i++)
	{
		fscanf(executable_pipes[i], "%f", &population::population[i].fitness);
		fprintf(stderr, "%d: fitness %f\n", i, population::population[i].fitness);
		fclose(executable_pipes[i]);
	}
}
