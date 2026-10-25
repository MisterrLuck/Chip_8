#include "assembler.hpp"

#include <iostream>
#include <format>
using std::cout, std::string, std::cerr, std::format;

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

	assembler.assemble();
	assembler.writeProgram("out.ch8");

	return 0;
}
