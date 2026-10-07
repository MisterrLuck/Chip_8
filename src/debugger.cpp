#include "debugger.hpp"

#include <iostream>
#include <algorithm>
using std::cin, std::cout;

void Debugger::init(Chip8* chip)
{
	chip8 = chip;

	getProgramEnd();
	// Add the location branching functions here
}


bool Debugger::prompt()
{
	locationBranch();
	return false;

	while (true)
	{
		char command;
		cout << ">>";
		cin >> command;

		// TODO: Maybe add update location branch command; In case the program updates memory
		// If it isn't expensive can maybe update it anyway
		// Or even add a test to see if there has been a change which is cheaper
		switch (command)
		{
			case 'n':
				return true;
			break;

			case 'l':
				cout << format("Last Opcode: 0x{:x}\n", chip8->opcode);
			break;

			case 'o':
				cout << format("Next Opcode: 0x{:x}\n", chip8->memory[chip8->pc] << 8 | chip8->memory[chip8->pc + 1]);
			break;

			case 'v': // Registers
				for (size_t ind = 0; ind <= 0xF; ind++)
				{
					cout << format("V{:x}: {:d}\n", ind, chip8->V[ind]);
				}
			break;

			// case 'm': // Memory
			// 	cout << chip8.opcode << "\n";
			// break;

			case 'i':
				cout << format("Index Register: 0x{:x}\n", chip8->I);
			break;

			case 'd':
				cout << format("Delay Timer: {:d}\n", chip8->delay_timer);
			break;

			case 's':
				cout << format("Sound Timer: {:d}\n", chip8->sound_timer);
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
}

void Debugger::locationBranch()
{
	// Do a loose branching of the program
	//  to be able to tell whats sprite data and whats not

	// Each opcode only has 1-2 options of a location
	//  most locations will be the address right after
	//  but jumps will have a different location
	//  and skips will have the extra possible address; hence branching

	// Any opcode not run has to be sprite data

	vector<unsigned short> unvisited_locations = { 0x200 };
	
	// Local lambda function
	auto addUnvisited = [this, &unvisited_locations](int address) {
		if (std::find(unvisited_locations.begin(), unvisited_locations.end(), address) == unvisited_locations.end())
		{
			if (std::find(locations.begin(), locations.end(), address) == locations.end())
			{
				cout << format("Adding 0x{:x}\n", address);
				unvisited_locations.push_back(address);
			}
		}
	};

	// As long as there are locations to visit
	while (unvisited_locations.size() > 0)
	{
		unsigned short curr_loc = unvisited_locations[0];

		// Add new locations based on the opcode at this location
		// Make sure to check if its already in the locations vector
		int opcode = chip8->getOpcode(curr_loc);
		cout << format("Opcode: 0x{:x}\n", opcode);

		// HACK: This might not be good
		if (opcode != 0)
		{
			switch (opcode & 0xF000)
			{
				// Skip statements
				case 0x3000:
				case 0x4000:
				case 0x5000:
				case 0x9000:
				case 0xE000: // Two instructions are both skips
					// Next and one after
					addUnvisited(curr_loc + 2);
					addUnvisited(curr_loc + 4);
				break;

				case 0x1000: // Jump
					addUnvisited(opcode & 0x0FFF); // Jump location
				break;

				case 0x2000: // Call subroutine: could be returned from
					addUnvisited(opcode & 0x0FFF); // Jump location
					addUnvisited(curr_loc + 2);
				break;

				case 0xB000: // This doesn't work with this opcode bc it will change

				break;

				default:
					addUnvisited(curr_loc + 2);
				break;
			}
		}

		// Add current locations to the list, and remove it from unvisited
		locations.push_back(curr_loc);
		unvisited_locations.erase(unvisited_locations.begin());
	}

	cin.ignore();

	for (auto loc : locations)
	{
		cout << format("Address: 0x{:x}\n", loc);
	}
	cout << "\n";
	printProgram();
}

void Debugger::printProgram()
{
	int address = 0x200;
	int opcode = chip8->getOpcode(address);
	
	while (address <= program_end)
	{
		// Found
		if (std::find(locations.begin(), locations.end(), address) != locations.end())
		{
			cout << format("O{{0x{:04x}}}: 0x{:04x}\n", address, opcode);
			address += 2;
		} else
		{
			cout << format("D{{0x{:04x}}}: 0x{:02x}\n", address, chip8->memory[address]);
			address++;
		}
		opcode = chip8->getOpcode(address);
	}
}

void Debugger::getProgramEnd()
{
	const int MEMORY_END = 4096;
	int curr_loc = 0x200;
	program_end = curr_loc;

	while (curr_loc != MEMORY_END)
	{
		if (chip8->memory[curr_loc] != 0)
			program_end = curr_loc;

		curr_loc++;
	}
}

void Debugger::remainingLocations()
{
	// int end_address = 0x200;
	// int opcode = chip8->getOpcode(end_address);
	//
	// while (opcode != 0)
	// {
	//
	// }
}
