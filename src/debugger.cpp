#include "debugger.hpp"

#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>
using std::cin, std::cout, std::string;

// TODO: Add prompt before the first opcode has run, especially to see if the person wants to run it yet; like in gdb
void Debugger::init(Chip8* chip)
{
	chip8 = chip;

	getProgramEnd();
	// Add the location branching functions here
	locationBranch();
}


bool Debugger::prompt()
{
	if (cont)
	{
		// TODO: Add message for which breakpoint hit

		// if pc not in breakpoints
		auto bp_ind = std::find(breakpoints.begin(), breakpoints.end(), chip8->pc);
		if (bp_ind == breakpoints.end())
			return true;
		cont = false;
		// TODO: Get rid of this after testing
		breakpoints.erase(bp_ind);
	}

	while (true)
	{
		char command;
		cout << ">>";
		cin >> command;

		// TODO: Maybe add update location branch command; In case the program updates memory
		// If it isn't expensive can maybe update it anyway
		// Or even add a test to see if there has been a change which is cheaper
		// TODO: Make this better; it sucks rn
		switch (command)
		{
			case 'p':
				printProgram();
			break;

			case 'n':
				return true;
			break;

			case 'l':
				// cout << format("Last Opcode: 0x{:x}\n", chip8->opcode);
				listProgram(20, chip8->pc);
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

			case 'a':
				cout << format("Address: 0x{:x}\n", chip8->pc);
			break;

			case 'b':
			{
				// NOTE: This wont accept hex
				string str_address;

				cout << "Break at 0x";
				cin >> str_address;
				
				unsigned int address = std::stoul(str_address, nullptr, 16);
				
				breakpoints.push_back(address);
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
				cont = true;
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
	vector<unsigned short> unvisited_locations = { 0x200 };
	
	// Local lambda function
	auto addUnvisited = [this, &unvisited_locations](int address) {
		if (std::find(unvisited_locations.begin(), unvisited_locations.end(), address) == unvisited_locations.end())
		{
			if (std::find(opcode_locations.begin(), opcode_locations.end(), address) == opcode_locations.end())
			{
				// cout << format("Adding 0x{:x}\n", address);
				unvisited_locations.push_back(address);
			}
		}
	};

	// As long as there are locations to visit
	while (unvisited_locations.size() > 0)
	{
		unsigned short curr_address = unvisited_locations[0];

		int opcode = chip8->getOpcode(curr_address);
		// cout << format("Opcode: 0x{:x}\n", opcode);

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
					addUnvisited(curr_address + 2);
					addUnvisited(curr_address + 4);
				break;

				case 0x1000: // Jump
					addUnvisited(opcode & 0x0FFF); // Jump location
				break;

				case 0x2000: // Call subroutine: could be returned from
					addUnvisited(opcode & 0x0FFF); // Jump location
					addUnvisited(curr_address + 2);
				break;

				// TODO: Find a way for 0xBNNN to work with this algorithm
				case 0xB000: // This doesn't work with this opcode bc it will change

				break;

				default:
					addUnvisited(curr_address + 2);
				break;
			}
		}

		// Add current locations to the list, and remove it from unvisited
		opcode_locations.push_back(curr_address);
		unvisited_locations.erase(unvisited_locations.begin());
	}
	std::sort(opcode_locations.begin(), opcode_locations.end());

	getDataLocations();
}

void Debugger::listProgram(int lines, int start_address)
{
	start_address = std::max(0x200, start_address);
	bool started = false;
	int line_count = 0;

	cout << "Program:\n\n";
	for (auto opcode_address : opcode_locations)
	{
		if (!started && start_address != opcode_address)
			continue;
		started = true;

		if (line_count >= lines && lines != -1)
			return;

		cout << format("{{0x{:04x}}}: {:04X} : {}\n", opcode_address, chip8->getOpcode(opcode_address), disassembleOpcode(chip8->getOpcode(opcode_address)));
		line_count++;
	}
}

void Debugger::printProgram()
{
	cout << "Program:\n\n";
	for (auto opcode_address : opcode_locations)
	{
		cout << format("{{0x{:04x}}}: {:04X} : {}\n", opcode_address, chip8->getOpcode(opcode_address), disassembleOpcode(chip8->getOpcode(opcode_address)));
	}

	cout << "\nData:\n";
	const int COUNT_LIMIT = 32;
	int current_count = 0;
	int last_data_address = -1;

	for (auto data_address : data_locations)
	{
		// Keep the successive sprite data together
		if (data_address == last_data_address+1)
		{
			if (current_count >= COUNT_LIMIT)
			{
				current_count = 0;
				cout << "\n            ";
			}
			cout << format("{:02x}", chip8->memory[data_address]);
		}
		else
		{
			current_count = 0;
			cout << format("\n{{0x{:04x}}}: 0x{:02x}", data_address, chip8->memory[data_address]);
		}

		current_count++;
		last_data_address = data_address;
	}
	cout << "\n";
}

void Debugger::getProgramEnd()
{
	const int MEMORY_END = 4096;
	int curr_address = 0x200;
	program_end = curr_address;

	while (curr_address != MEMORY_END)
	{
		if (chip8->memory[curr_address] != 0)
			program_end = curr_address;

		curr_address++;
	}
}

void Debugger::getDataLocations()
{
	int curr_address = 0x200;
	
	while (curr_address <= program_end)
	{
		// Found
		if (std::find(opcode_locations.begin(), opcode_locations.end(), curr_address) != opcode_locations.end())
		{
			curr_address += 2;
		} else
		{
			data_locations.push_back(curr_address);
			// cout << format("D{{0x{:04x}}}: 0x{:02x}\n", address, chip8->memory[address]);
			curr_address++;
		}
	}
}

string Debugger::disassembleOpcode(int opcode)
{
	int X = (opcode >> 8) & 0xF;
	int Y = (opcode >> 4) & 0xF;
	int N    = opcode & 0x000F;
	int NN   = opcode & 0x00FF;
	int NNN  = opcode & 0x0FFF;
		
	switch (opcode & 0xF000)
	{
		case 0x0000:
			switch (opcode & 0x0FFF)
			{
				case 0x00E0: // 0x00E0: Clears screen
					return "CLS";
				break;

				case 0x00EE: // 0x00EE: Return from subroutine
					return "RET";
				break;
			}
		break;

		case 0x1000: // 0x1NNN : jump to address 
			return format("JMP {:x}", NNN);
		break;

		case 0x2000: // 0x2NNN : jump to subroutine at address
			return format("CALL {:x}", NNN);
		break;

		case 0x3000: // 0x3XNN : skip next instruction if VX == NN
			return format("SKE V{:X}, {:x}", X, NN);
		break;

		case 0x4000: // 0x4XNN : skip next instruction if VX != NN
			return format("SKNE V{:X}, {:x}", X, NN);
		break;

		case 0x5000: // 0x5XY0 : skip next instruction if VX == VY
			return format("SKE V{:X}, V{:X}", X, Y);
		break;

		case 0x6000: // 0x6XNN : sets VX = NN
			return format("STR V{:X}, {:x}", X, NN);
		break;

		case 0x7000: // 0x7XNN : adds value NN to register VX
			return format("ADD V{:X}, NN", X, NN);
		break;

		case 0x8000:
			switch (opcode & 0x000F)
			{
				case 0x0000: // 0x8XY0 : store VY in VX
					return format("STR V{:X}, V{:X}", X, Y);
				break;

				case 0x0001: // 0x8XY1 : VX = VX OR VY
					return format("OR V{:X}, V{:X}", X, Y);
				break;

				case 0x0002: // 0x8XY2 : VX = VX AND VY
					return format("AND V{:X}, V{:X}", X, Y);
				break;

				case 0x0003: // 0x8XY3 : VX = VX XOR VY
					return format("XOR V{:X}, V{:X}", X, Y);
				break;

				case 0x0004: // 0x8XY4 : VX = VX + VY with carry
					return format("ADD V{:X}, V{:X}", X, Y);
				break;

				case 0x0005: // 0x8XY5 : VX - VY with underflow
					return format("SUBR V{:X}, V{:X}", X, Y);
				break;

				case 0x0006: // 0x8XY6 : shift VX right one, move bit to VF
					if (chip8->OG_SHIFT)
						return format("SHR V{:X}, V{:X}", X, Y);
					else
						return format("SHR V{:X}", X);
				break;

				case 0x0007: // 0x8XY7 : VY - VX with underflow
					return format("SUBL V{:X}, V{:X}", X, Y);
				break;

				case 0x000E: // 0x8XYE : shift VX left one, move bit to VF
					if (chip8->OG_SHIFT)
						return format("SHL V{:X}, V{:X}", X, Y);
					else
						return format("SHL V{:X}", X);
				break;
			}
		break;

		case 0x9000: // 0x9XY0 : Skep next if VX != VY
			return format("SKNE V{:X}, V{:X}", X, Y);
		break;

		case 0xA000: // 0xANNN : sets I to address NNN
			return format("STR I, {:x}", NNN);
		break;

		case 0xB000: // 0xBNNN or 0xBXNN : jumps to NNN/XNN with an offset of V0/VX
			if (chip8->OG_JUMP_OFFSET)
				return format("JMPO {:x}", NNN);
			else
				return format("JMPO V{:X}, {:x}", X, NNN);
		break;

		case 0xC000: // 0xCXNN : sets VX to a random number between 0 and 255 ANDED with NN
			return format("RND V{:X}, NN", X, NN);
		break;

		case 0xD000: // 0xDXYN : draw a sprite at (X,Y)
			return format("DRAW V{:X}, V{:X}", X, Y);
		break;

		// Key instructions
		case 0xE000:
			switch (opcode & 0x00FF)
			{
				case 0x009E: // EX9E : skips next instruction if key in VX is pressed
					return format("SKK V{:X}", X);
				break;

				case 0x00A1: // EXA1 : skips next instruction if key in VX is not pressed
					return format("SKNK V{:X}", X);
				break;
			}
		break;

		case 0xF000: //0xFX..
			switch (opcode & 0x00FF)
			{
				case 0x0007: // 0xFX07 : Store value of delay timer in VX
					return format("STR V{:X}, DT", X);
				break;

				case 0x000A: // 0xFX0A : Wait until key is pressed
					return format("KEY V{:X}", X);
				break;

				case 0x0015: // 0xFX15 : Set value of delay timer to VX
					return format("STR DT, V{:X}", X);
				break;

				case 0x0018: // 0xFX18 : Set value of sound timer to VX
					return format("STR ST, V{:X}", X);
				break;

				case 0x001E: // 0xFX1E : Add VX to index register I
					return format("ADD I, V{:X}", X);
				break;

				case 0x0029: // 0xFX29 : Set I to the font address of the hexadecimal character in VX
					return format("CHAR V{:X}", X);
				break;

				case 0x0033: // 0xFX33 : stores value in VX as BCD
					return format("BCD V{:X}", X);
				break;

				case 0x0055: // 0xFX55 : Loads registers 0-X inclusive in memory at I
					return format("SMEM V{:X}", X);
				break;

				case 0x0065: // 0xFX65 : Store the memory from I into registers 0-X
					return format("LMEM V{:X}", X);
				break;
			}
		break;
	}

	return "UNKNOWN";
}
