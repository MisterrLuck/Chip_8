#pragma once
#include <string>
#include <vector>
#include <tuple>
using std::string, std::vector, std::tuple;

class Assembler
{
public:
	bool loadProgram(string file_name);
	string cleanLine(string line); // removes spaces and comments and such in lines

	// Without the comments and spaces, but not tokenized yet ?
	vector<string> file_contents;

	vector<tuple<string, int>> label_locations;
};

string trim(const string& str, const string& whitespace = " \t");
