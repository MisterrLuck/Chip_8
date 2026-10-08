#pragma once
#include "chip8.hpp"

#include <format>
#include <vector>
#include <string>
using std::format, std::vector, std::string;

class Debugger
{
public:
	void init(Chip8* chip);
	// Returns false if requesting to exit the program
	bool prompt();			// Prompts the user for commands

private:
	bool cont = false; // when called the continue command; only stop for breakpoints

	Chip8* chip8;

	// Last address with information in the program; inclusive
	int program_end;

	int print_line_count = 0;
		
	vector<unsigned short> breakpoints;
	// vector<unsigned short[2]> disassembled_code;
	
	// For the location branching
	vector<unsigned short> opcode_locations;
	vector<unsigned short> data_locations;


	// NOTE: Should only be run once bc it's probably expensive
	// TODO: Store the program directly instead of grabbing from memory?
	// so that it can run seperately from the debugger
	void disassemble();
	string disassembleOpcode(int opcode);
	
	// My searching algorithm idea
	// NOTE: Maybe needs a better name
	void locationBranch();
	void getDataLocations();

	void printProgram(int lines=-1, int start_address=0x200);
	void getProgramEnd();
};
