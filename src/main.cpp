#include <iostream>
#include <chrono>
#include <thread>
#include <string>

#ifdef DEBUGGER
#include "debugger.hpp"
#endif

#include "chip8.hpp"
#include "graphics.hpp"
using std::cout, std::string, std::cerr;


int main(int argc, char *argv[])
{

	Chip8 chip8;
	Graphics graphics;
	
// #ifndef DEBUGGER
	// Get rid of raylib output
	SetTraceLogLevel(LOG_ERROR); 
// #endif
	
	const int INST_PER_SECOND = 700; // 700 instructions per second
	const int MICROSECONDS_PER_INST = 1000000 / INST_PER_SECOND;

	const int CLOCK_TICK_PER_SECOND = 60; // 60 Hz (60 times per second)
	const int MICROSECONDS_PER_CLOCK_TICK = 1000000 / CLOCK_TICK_PER_SECOND;

	// Initialization
	chip8.init();
	graphics.init(20, ORANGE, MAROON);

	if (argc > 1)
		chip8.loadProgram(string(argv[1]));
	else
	{
		// TODO: Include default program to run
		cout << "No program provided\n";
		return -1;
	}
	
#ifdef DEBUGGER
	Debugger debugger;
	debugger.init(&chip8);
#endif

	while (!WindowShouldClose())
	{
		// pythons time.time()
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

		chip8.emulateCycle();

		PollInputEvents();
		if (chip8.draw_flag)
			graphics.updateFrame(chip8.gfx);

		// Timing for ~accurate speed of processing
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		int inst_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

		int remaining_inst_time = MICROSECONDS_PER_INST - inst_duration;

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
