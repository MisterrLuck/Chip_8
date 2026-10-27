#include "CLI11.hpp"
#include "config.hpp"
#include "assembler.hpp"

#include <iostream>
#include <format>
using std::cout, std::string, std::cerr, std::format;


void argsSettings(CLI::App* app, int argc, char *argv[]);

// Argument Variables
string input_file = "";
string output_file = "out.ch8";

bool cosmic_flag = false;
bool super_flag = false;


// TODO: Use std::filesystem to have default output be the input without .asm -> .ch8
int main(int argc, char *argv[])
{
	// Argument Parser
	CLI::App app{"Chip 8: Ver 2.1"};
	argsSettings(&app, argc, argv);
	CLI11_PARSE(app, argc, argv);

	// Init
	Config config = flagsToConfig(cosmic_flag, super_flag);

	Assembler assembler(config);

	// Loading the Program
	bool loaded = assembler.loadProgram(input_file);

	if (!loaded)
		return -1;

	// Assemble
	assembler.assemble();
	assembler.writeProgram(output_file);

	return 0;
}


void argsSettings(CLI::App* app, int argc, char *argv[])
{
	argv = app->ensure_utf8(argv);

	app->add_option("file_path", input_file, "Program to assemble")
		->required();

	// Main Options
	app->add_option("-o,--output", output_file, "Output file name")
		->capture_default_str();


	// Configuration Options
	#define CONFIG_GROUP_NAME "Configuration Options: Only one flag can be set"

	app->add_flag("--cosmic", cosmic_flag, "Run in COSMIC configuration")
		->group(CONFIG_GROUP_NAME)
		->ignore_case();

	app->add_flag("--super,--super-chip", super_flag, "Run in SUPER_CHIP configuration")
		->group(CONFIG_GROUP_NAME)
		->ignore_case();
}
