#pragma once
#include <string>
#include <vector>
#include <tuple>
using std::string, std::vector, std::tuple;

class Assembler
{
public:
	bool loadProgram(string file_name);

	// removes spaces and comments and such in lines
	string cleanLine(string line);
	
	// IDK what to call this function
	void locateLabels();

	// Get how many bytes a line will occupy
	// usually only 2, unless its the BYTES instruction
	int getOffset(string line);
	
	void writeProgram(string file_name);

	// Without the comments and spaces, but not tokenized yet ?
	vector<string> file_contents;

	vector<tuple<string, int>> label_locations;
};

string strToLower(string str);
string trim(const string& str, const string& whitespace = " \t");
