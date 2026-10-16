#include "assembler.hpp"

#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
using std::string;

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
	// line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
	line = trim(line);


	return line;
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
