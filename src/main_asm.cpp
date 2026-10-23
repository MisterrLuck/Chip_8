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

	for (tuple<string, int> label : assembler.label_locations)
		cout << format("{}: 0x{:x}\n", get<0>(label), get<1>(label));

	cout << "\n";

	for (string line : assembler.file_contents)
		cout << line << "\n";

	cout << "\n";

	for (int opcode : assembler.binary_program)
		cout << format("{:02X}\n", opcode);

	return 0;
}
