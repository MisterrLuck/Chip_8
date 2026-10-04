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
	SetTraceLogLevel(LOG_ERROR); 

	Chip8 chip8;
	Graphics graphics;

	// Initialization
	chip8.init();
	
	if (!graphics.init(20))
	{
		cout << "Error with the graphics\n";
		return -1;
	}

	// chip8.loadProgram("../roms/IBM_logo.ch8");
	// chip8.loadProgram("../roms/debug.ch8");
	// chip8.loadProgram("../roms/octo.ch8");
	// chip8.loadProgram("../roms/octojam3title.ch8");
	if (argc > 1)
		chip8.loadProgram(string(argv[1]));
	else
		chip8.loadProgram("../roms/IBM_logo.ch8");

#ifdef DEBUGGER
	bool debugging = true;
#endif

	while (!WindowShouldClose())
	{
		chip8.emulateCycle();
		
		if (chip8.draw_flag)
			graphics.updateFrame(chip8.gfx);

// Slow down per instruction and allow for printing of memory and registers
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
