#pragma once
#include "chip8.hpp"

#include <vector>
using std::vector;

class Debug
{
public:
	void init(Chip8* chip);
	// Returns false if requesting to exit the program
	bool prompt();			// Prompts the user for commands

private:
	// NOTE: Should only be run once bc it's probably expensive
	void disassemble();		// The sprite data will be wrongly interpreted

	vector<short> breakpoints;
	vector<short[2]> disassembled_code;

	Chip8* chip8;
}
