#pragma once
#include "chip8.hpp"

#include <format>
#include <vector>
using std::format, std::vector;

class Debugger
{
public:
	void init(Chip8* chip);
	// Returns false if requesting to exit the program
	bool prompt();			// Prompts the user for commands

private:
	Chip8* chip8;

	// vector<unsigned short> breakpoints;
	// vector<unsigned short[2]> disassembled_code;
	
	// For the location branching
	vector<unsigned short> locations;
	vector<unsigned short> data_locations;

	// Last address with information in the program; inclusive
	int program_end;


	// NOTE: Should only be run once bc it's probably expensive
	// TODO: Store the program directly instead of grabbing from memory?
	// so that it can run seperately from the debugger
	void disassemble();
	
	// My searching algorithm idea
	// NOTE: Maybe needs a better name
	void locationBranch();
	void getDataLocations();

	void printProgram();
	void getProgramEnd();
};
