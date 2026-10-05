#include <iostream>
#include <unistd.h>
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
		chip8.emulateCycle();
		
		PollInputEvents();
		if (chip8.draw_flag)
			graphics.updateFrame(chip8.gfx);

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
