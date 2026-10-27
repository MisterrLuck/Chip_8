#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <vector>

#include "CLI11.hpp"
#include "config.hpp"

#include "debugger.hpp"
#include "chip8.hpp"
#include "graphics.hpp"
using std::cout, std::string, std::cerr, std::vector;


const int INST_PER_SECOND = 700; // 700 instructions per second
const int MICROSECONDS_PER_INST = 1000000 / INST_PER_SECOND;

const int CLOCK_TICK_PER_SECOND = 60; // 60 Hz (60 times per second)
const int MICROSECONDS_PER_CLOCK_TICK = 1000000 / CLOCK_TICK_PER_SECOND;


void argsSettings(CLI::App* app, int argc, char *argv[]);

// Argument Variables
string file_name = "";

bool debug_flag = false;
Config config = Config::COSMIC;

vector<unsigned char> background = {0, 0, 0};
vector<unsigned char> foreground = {0xFF, 0xFF, 0xFF};
int pixel_size = 20;


// TODO: Use raygui when combining the debugger such into one window (https://github.com/raysan5/raygui)
// TODO: Interesting chip 8 binary thing (https://github.com/Timendus/chip8-binary-format)
// TODO: Aseprite to binary data addon
int main(int argc, char *argv[])
{
	SetTraceLogLevel(LOG_ERROR); 
	
	// Argument Parser
	CLI::App app{"Chip 8: Ver 2.1"};
	argsSettings(&app, argc, argv);
	CLI11_PARSE(app, argc, argv);

	Color background_color = (Color) {background[0], background[1], background[2], 255};
	Color foreground_color = (Color) {foreground[0], foreground[1], foreground[2], 255};


	// Initialization
	Chip8 chip8;
	chip8.init(config);

	Graphics graphics;
	graphics.init(pixel_size, background_color, foreground_color);

	Debugger debugger;


	// Load the Program
	bool loaded = chip8.loadProgram(file_name);

	if (!loaded)
		return -1;
	

	// Setup Debugger
	if (debug_flag)
	{
		debugger.init(&chip8);
		if (!debugger.prompt())
			return 0;
	}

	// Main Loop
	while (!WindowShouldClose())
	{
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();


		chip8.emulateCycle();

		PollInputEvents();
		if (chip8.draw_flag)
			graphics.updateFrame(chip8.gfx);


		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		int inst_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

		int remaining_inst_time = MICROSECONDS_PER_INST - inst_duration;

		// MICROSECONDS_PER_INST is just the amount of time that has passed
		chip8.updateTimers(MICROSECONDS_PER_CLOCK_TICK, MICROSECONDS_PER_INST);

		// Have a small range instead of just 0
		if (remaining_inst_time > 5)
			// Sleep for the remaining time to attempt ints per second
			std::this_thread::sleep_for(std::chrono::microseconds(remaining_inst_time));


		if (debug_flag)
		{
			if (!debugger.prompt())
				return 0;
		}
	}
	
	return 0;
}


void argsSettings(CLI::App* app, int argc, char *argv[])
{
	argv = app->ensure_utf8(argv);

	app->add_option("file_path", file_name, "Program to run")
		->required();

	// Main Options
	app->add_flag("-d,--debug", debug_flag, "Run the program in debug");


	// Configuration Options
    app->add_option("-c,--config", config, "Configuration Version")
        ->required()
        ->transform(CLI::CheckedTransformer(CONFIG_CLI_MAP, CLI::ignore_case));


	// Extra Options
	app->add_option("--background", background, "Change the background color")
		->group("Extra Options")
		->expected(3);

	app->add_option("--foreground", foreground, "Change the foreground color")
		->group("Extra Options")
		->expected(3);

	app->add_option("--size,--pixel-size", pixel_size, "Change the pixel size")
		->group("Extra Options")
		->capture_default_str();
}
