#include <iostream>
#include <chrono>
#include <thread>
#include <string>

#ifdef DEBUGGER
#include <format>
using std::format;
#endif

#include "graphics.hpp"
#include "chip8.hpp"
using std::cout, std::string, std::cin;


int main(int argc, char *argv[])
{
	// Get rid of raylib output
#ifndef DEBUGGER
	SetTraceLogLevel(LOG_ERROR); 
#endif

	Chip8 chip8;
	Graphics graphics;
	
	const int INST_PER_SECOND = 700;
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
		cout << "No program provided\n";
		return -1;
	}

#ifdef DEBUGGER
	bool debugging = true;
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

// Custom one by one debugger
#ifdef DEBUGGER
		if (!debugging)
			continue;

		bool loop = true;
		while (loop)
		{
			char command;
			cout << ">>";
			cin >> command;

			switch (command)
			{
				case 'n':
					loop = false;
				break;

				case 'l':
					cout << format("Last Opcode: 0x{:x}", chip8.opcode) << "\n";
				break;

				case 'o':
					cout << format("Next Opcode: 0x{:x}", chip8.memory[chip8.pc] << 8 | chip8.memory[chip8.pc + 1]) << "\n";
				break;

				case 'v': // Registers
					for (size_t ind = 0; ind <= 0xF; ind++)
					{
						cout << format("V{:x}: {:d}", ind, chip8.V[ind]) << "\n";
					}
				break;

				// case 'm': // Memory
				// 	cout << chip8.opcode << "\n";
				// break;

				case 'i':
					cout << format("Index Register: 0x{:x}", chip8.I) << "\n";
				break;

				case 'd':
					cout << format("Delay Timer: {:d}", chip8.delay_timer) << "\n";
				break;

				case 's':
					cout << format("Sound Timer: {:d}", chip8.sound_timer) << "\n";
				break;

				case 'c':
					debugging = false;
					loop = false;
				break;

				case 'e':
					return 0;
				break;
			}
			cout << "\n";
		}
#endif
	}
	
	return 0;
}
