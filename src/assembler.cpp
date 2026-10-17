#include "assembler.hpp"

#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <format>
using std::string, std::format, std::cout;

bool Assembler::loadProgram(string file_name)
{
	std::ifstream file(file_name);
	if (!file.is_open())
	{
		std::cerr << "Unable to find program\n";
		return false;
	}

	string line;
	while (std::getline(file, line))
	{
		string new_line = cleanLine(line);
		if (new_line != "")
			file_contents.push_back(new_line);
	}

	file.close();

	return true;

}


string Assembler::cleanLine(string line)
{
	// Removing comments
	// TODO: Make this work for semicolon comments as well
	int hash_ind = line.find('#');
	// int semi_ind = line.find(';');
	// std::cout << hash_ind << " " << semi_ind << "\n";

	// int ind = std::min(hash_ind, semi_ind);

	if (hash_ind != string::npos)
		line = line.substr(0, hash_ind);

	// Remove excess whitespace
	line = trim(line);

	return line;
}

void Assembler::locateLabels()
{
	int pc = 0x200;
	for (int line_ind = 0; line_ind < file_contents.size(); line_ind++)
	{
		string line = file_contents[line_ind];

		// HACK: Add more checking to prove this is a label!!!!!
		if (*(line.end()-1) == ':')
		// check if current line is a label
		{
			// TODO: Add Error checking for the label names

			tuple<string, int> label_tuple(line.substr(0, line.size()-1), pc);
			label_locations.push_back(label_tuple);

			// Remove the label in the file
			file_contents.erase(file_contents.begin() + line_ind);
			line_ind--;
		} else
		{
			int offset = getOffset(line);
			// cout << format("{}\n", offset);
			pc += offset;
		}

		// cout << format("{{0x{:x}}}\n", pc);
	}

}

int Assembler::getOffset(string line)
{
	string instruction;
	int ind = line.find(' ');

	if (ind != string::npos)
		instruction = line.substr(0, ind);
	else // instructions with no 
		instruction = line;

	instruction = strToLower(instruction);

	if (instruction != "bytes")
		return 2;

	// Count how many bytes the bytes instruction is storing
	// HACK: This is probably bad practice; but the error checking will be stronger at a later stage?
	return count(line.begin(), line.end(), ',')	+ 1;
}

string strToLower(string str)
{
	std::transform(str.begin(), str.end(), str.begin(),
    [](unsigned char c){ return std::tolower(c); });
	return str;
}

// https://stackoverflow.com/questions/1798112/removing-leading-and-trailing-spaces-from-a-string
string trim(const string& str, const string& whitespace)
{
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}
