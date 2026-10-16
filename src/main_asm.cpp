#include "assembler.hpp"

#include <iostream>
using std::cout, std::string, std::cerr;

int main(int argc, char *argv[])
{
	Assembler assembler;

	if (argc > 1)
	{
		bool loaded = assembler.loadProgram(string(argv[1]));

		if (!loaded)
			return -1;
	}
	else
	{
		cerr << "No program provided\n";
		return -1;
	}

	for (string line : assembler.file_contents)
		cout << line << "\n";


	return 0;
}
