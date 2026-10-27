#include <iostream>
#include <chrono>
#include <thread>
#include <string>

#include "CLI11.hpp"

#ifdef DEBUGGER
#include "debugger.hpp"
#endif

#include "chip8.hpp"
#include "graphics.hpp"
using std::cout, std::string, std::cerr;


const int INST_PER_SECOND = 700; // 700 instructions per second
const int MICROSECONDS_PER_INST = 1000000 / INST_PER_SECOND;

const int CLOCK_TICK_PER_SECOND = 60; // 60 Hz (60 times per second)
const int MICROSECONDS_PER_CLOCK_TICK = 1000000 / CLOCK_TICK_PER_SECOND;


// TODO: Use raygui when combining the debugger such into one window (https://github.com/raysan5/raygui)
// TODO: Interesting chip 8 binary thing (https://github.com/Timendus/chip8-binary-format)
// TODO: Aseprite to binary data addon
int main(int argc, char *argv[])
{
	SetTraceLogLevel(LOG_ERROR); 
	
	// Argument Parser
	CLI::App app{"Chip 8: v2.1"};
	argv = app.ensure_utf8(argv);

	string file_name = "";
	app.add_option("file", file_name, "Program to run")
		->required();

	// Main Options
	bool debug_flag = false;
	app.add_flag("-d,--debug", debug_flag, "Run the debugger")
		->group("Main Options");


	// Configuration Options
	bool cosmic_flag = false;
	app.add_flag("--cosmic", cosmic_flag, "Run in COSMIC configuration")
		->group("Configuration Options")
		->ignore_case();

	bool super_flag = false;
	app.add_flag("--super,--super-chip", super_flag, "Run in SUPER_CHIP configuration")
		->group("Configuration Options")
		->ignore_case();


	// Extra Options
	string background = "#000000";
	app.add_option("--background", background, "Change the background color")
		->group("Extra Options")
		->capture_default_str();

	string foreground = "#FFFFFF";
	app.add_option("--foreground", foreground, "Change the foreground color")
		->group("Extra Options")
		->capture_default_str();

	int pixel_size = 20;
	app.add_option("--size,--pixel-size", pixel_size, "Change the pixel size")
		->group("Extra Options")
		->capture_default_str();

	CLI11_PARSE(app, argc, argv);

	return 0;

	// Initialization
	Chip8 chip8;
	chip8.init();

	Graphics graphics;
	graphics.init(20, ORANGE, MAROON);


	if (argc > 1)
	{
		bool loaded = chip8.loadProgram(string(argv[1]));

		if (!loaded)
			return -1;
	}
	else
	{
		cerr << "No program provided\n";
		return -1;
	}
	
#ifdef DEBUGGER
	Debugger debugger;
	debugger.init(&chip8);
	if (!debugger.prompt())
		return 0;
#endif

	while (!WindowShouldClose())
	{
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

		chip8.emulateCycle();

		PollInputEvents();
		if (chip8.draw_flag)
			graphics.updateFrame(chip8.gfx);

		// TODO: Without draw flag, there is a speed issue; which also affects the timers
		// Timing for ~accurate speed of processing
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		int inst_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

		int remaining_inst_time = MICROSECONDS_PER_INST - inst_duration;
		cout << remaining_inst_time << "\n";

		// MICROSECONDS_PER_INST is just the amount of time that has passed
		chip8.updateTimers(MICROSECONDS_PER_CLOCK_TICK, MICROSECONDS_PER_INST);

		// Have a small range instead of just 0
		if (remaining_inst_time > 5)
			// Sleep for the remaining time to attempt ints per second
			std::this_thread::sleep_for(std::chrono::microseconds(remaining_inst_time));

		// cout << remaining_inst_time << "\n";

#ifdef DEBUGGER
		if (!debugger.prompt())
			return 0;
#endif
	}
	
	return 0;
}

