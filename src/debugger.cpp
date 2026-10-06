#include "debugger.hpp"

#include <iostream>
using std::cin;

void Debugger::init(Chip8* chip)
{
	chip8 = chip;
}


bool Debugger::prompt()
{
	while (true)
	{
		char command;
		cout << ">>";
		cin >> command;

		switch (command)
		{
			case 'n':
				return true;
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

			// TODO: Fix `c` command
			case 'c':
				return true;
			break;

			case 'e':
				return false;
			break;
		}
		cout << "\n";
	}
	return true;
}

void Debugger::disassemble()
{
	// Do a loose branching of the program
	//  to be able to tell whats sprite data and whats not

	// Each opcode only has 1-2 options of a location
	//  most locations will be the address right after
	//  but jumps and skips will have the extra possible address; hence branching

	// Any opcode not run has to be sprite data
}
