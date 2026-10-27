#pragma once
#include <string>
#include <vector>
#include <tuple>

#include "CLI11.hpp"
#include "config.hpp"
using std::string, std::vector, std::tuple;


#ifdef COSMIC
#define SHIFT
#define JUMP_OFFSET
#define REG_MEMORY_INDEX
#define BOOLEAN_RESET
#endif

#ifdef SUPER_CHIP
#undef SHIFT
#undef JUMP_OFFSET
#undef REG_MEMORY_INDEX
#undef BOOLEAN_RESET
#endif


string strToLower(string str);
string strip(const string& str, const string& whitespace = " \t");

class Assembler
{
public:
	Assembler(Config config);
	void setConfig(Config config);

	bool loadProgram(string file_name);
	void assemble();
	void writeProgram(string file_name);

// private:
	// removes spaces and comments and such in lines
	string cleanLine(string line);
	
	// IDK what to call this function
	void locateLabels();

	// Get how many bytes a line will occupy
	// usually only 2, unless its the BYTES instruction
	int getOffset(string line);

	tuple<string, vector<string>> splitInstruction(string instruction);
	int assembleInstruction(string instruction);
	

	// Helper functions
	int getLabelAddress(string label);
	int getRegisterNum(string reg);
	int decodeNN(string num);
	int decodeN(string num);

	// Without the comments and spaces, and eventually without the labels
	vector<string> file_contents;
	vector<char> binary_program;
	vector<tuple<string, int>> label_locations;

	// single byte flag for the BYTES instruction
	bool single_byte;


	// Configurations
	bool OG_SHIFT;
	bool OG_JUMP_OFFSET;
	bool OG_BOOLEAN_RESET;
};

