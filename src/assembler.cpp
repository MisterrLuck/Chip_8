#include "assembler.hpp"

#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <format>
#include <sstream>
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

void Assembler::assemble()
{
	locateLabels();

	for (string instruction : file_contents)
	{
		int opcode = assembleInstruction(instruction);

		if (!single_byte)
			binary_program.push_back((opcode & 0xFF00) >> 8);
		binary_program.push_back(opcode & 0xFF);
	}
}

void Assembler::writeProgram(string file_name)
{
	#include <fstream>

	std::ofstream fout;
	fout.open(file_name, std::ios::binary | std::ios::out);

	fout.write((char*) &binary_program[0], binary_program.size());

	fout.close();
}

int Assembler::assembleInstruction(string instruction)
{
	single_byte = false;

	auto i = splitInstruction(instruction);
	instruction = get<0>(i);
	vector<string> parameters = get<1>(i);

	// TODO: Add to lower for the instruction and parameters
	// TODO: Add strict tests on parameter length for all instructions, and parameters
	if (instruction == "CLS")
		return 0x00E0;
	if (instruction == "RET")
		return 0x00EE;
	if (instruction == "JUMP")
		return 0x1000 | getLabelAddress(parameters[0]);
	if (instruction == "CALL")
		return 0x2000 | getLabelAddress(parameters[0]);
	if (instruction == "SKE")
	{
		// Move the VX register into place
		int opcode = getRegisterNum(parameters[0]) << 8;
		if (getRegisterNum(parameters[1]) == -1)
			return 0x3000 | opcode | decodeNN(parameters[1]);
		return 0x5000 | opcode | (getRegisterNum(parameters[1]) << 4);
	} if (instruction == "SKNE")
	{
		// Move the VX register into place
		int opcode = getRegisterNum(parameters[0]) << 8;
		if (getRegisterNum(parameters[1]) == -1)
			return 0x4000 | opcode | decodeNN(parameters[1]);
		return 0x9000 | opcode | (getRegisterNum(parameters[1]) << 4);
	} if (instruction == "STOR")
	{
		if (parameters[0] == "I")
			return 0xA000 | getLabelAddress(parameters[1]);
		if (parameters[0] == "DT")
			return 0xF015 | (getRegisterNum(parameters[1]) << 8);
		if (parameters[0] == "ST")
			return 0xF018 | (getRegisterNum(parameters[1]) << 8);
		else
		{
			if (parameters[1] == "DT")
				return 0xF007 | (getRegisterNum(parameters[0]) << 8);
			if (getRegisterNum(parameters[1]) == -1)
				return 0x6000 | (getRegisterNum(parameters[0]) << 8) | decodeNN(parameters[1]);
			return 0x8000 | (getRegisterNum(parameters[0]) << 8) | (getRegisterNum(parameters[1]) << 4);
		}
	} if (instruction == "ADD")
	{
		if (parameters[0] == "I")
			return 0xF01E | (getRegisterNum(parameters[1]) << 8);
		if (getRegisterNum(parameters[1]) == -1)
			// TODO: Fix this?? getting a -0FA for 7006
			return 0x7000 | (getRegisterNum(parameters[0]) << 8) | decodeNN(parameters[1]);
		return 0x8004 | (getRegisterNum(parameters[0]) << 8) | (getRegisterNum(parameters[1]) << 4);
	}  if (instruction == "OR")
		return 0x8001 | (getRegisterNum(parameters[0]) << 8) | (getRegisterNum(parameters[1]) << 4);
	if (instruction == "AND")
		return 0x8002 | (getRegisterNum(parameters[0]) << 8) | (getRegisterNum(parameters[1]) << 4);
	if (instruction == "XOR")
		return 0x8003 | (getRegisterNum(parameters[0]) << 8) | (getRegisterNum(parameters[1]) << 4);
	if (instruction == "SUBR")
		return 0x8005 | (getRegisterNum(parameters[0]) << 8) | (getRegisterNum(parameters[1]) << 4);
	if (instruction == "SUBL")
		return 0x8007 | (getRegisterNum(parameters[0]) << 8) | (getRegisterNum(parameters[1]) << 4);
	if (instruction == "RAND")
		return 0xC000 | (getRegisterNum(parameters[0]) << 8) | decodeNN(parameters[1]);
	if (instruction == "DRAW")
		return 0xD000 | (getRegisterNum(parameters[0]) << 8) | (getRegisterNum(parameters[1]) << 4) | decodeN(parameters[2]);
	if (instruction == "SKK")
		return 0xE09E | (getRegisterNum(parameters[0]) << 8);
	if (instruction == "SKNK")
		return 0xE0A1 | (getRegisterNum(parameters[0]) << 8);
	if (instruction == "KEY")
		return 0xF00A | (getRegisterNum(parameters[0]) << 8);
	if (instruction == "CHAR")
		return 0xF029 | (getRegisterNum(parameters[0]) << 8);
	if (instruction == "BCD")
		return 0xF033 | (getRegisterNum(parameters[0]) << 8);
	if (instruction == "SMEM")
		return 0xF055 | (getRegisterNum(parameters[0]) << 8);
	if (instruction == "LMEM")
		return 0xF065 | (getRegisterNum(parameters[0]) << 8);
	if (instruction == "BYTES")
	{
		// HACK: This is a mess but oh well
		for (int ind = 0; ind < parameters.size()-1; ind++)
			binary_program.push_back(decodeNN(parameters[ind]));

		single_byte = true;
		return decodeNN(parameters[parameters.size()-1]);
	}

	return 0;
}

tuple<string, vector<string>> Assembler::splitInstruction(string instruction)
{
	vector<string> parameters;

	int ind = instruction.find(' ');

	if (ind != string::npos)
	{
		string parameter_str = instruction.substr(ind+1, instruction.size() - (ind+1));
		instruction = instruction.substr(0, ind);
		
		string tmp;
		std::stringstream s(parameter_str);

		while (getline(s, tmp, ','))
			parameters.push_back(strip(tmp));

	}

	return tuple<string, vector<string>>(instruction, parameters);
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
	line = strip(line);

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
			pc += offset;
		}
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

int Assembler::getLabelAddress(string label)
{
	for (tuple<string, int> tup : label_locations)
	{
		if (get<0>(tup) == label)
			return get<1>(tup);
	}

	throw std::invalid_argument(format("Label doesn't exist: '{}'", label));
}

int Assembler::getRegisterNum(string reg)
{
	if (reg[0] != 'V' || reg.size() != 2)
		return -1;

	char num = reg[1];

	// 0-9
	if (num >= 48 && num <= 57)
		return num - 48;
	// A-F
	else if (num >= 65 && num <= 70)
		return num - 55;
	// a-f
	else if (num >= 97 && num <= 102)
		return num - 87;
	else
		return -1;
}

int Assembler::decodeNN(string num)
{
	unsigned int n;
	if (num.substr(0, 2) == "0x")
		n = std::stoul(num, nullptr, 16);
	else if (num.substr(0, 2) == "0b")
		n = std::stoul(num, nullptr, 2);
	else
		n = std::stoul(num, nullptr, 10);

	if (n > 0xFF)
		throw std::invalid_argument(format("Number is over 2 bytes: '{}'", n));

	return n;
}

int Assembler::decodeN(string num)
{
	unsigned int n;
	if (num.substr(0, 2) == "0x")
		n = std::stoul(num, nullptr, 16);
	else if (num.substr(0, 2) == "0b")
		n = std::stoul(num, nullptr, 2);
	else
		n = std::stoul(num, nullptr, 10);

	if (n > 0xF)
		throw std::invalid_argument(format("Number is over 1 byte: '{}'", n));

	return n;
}

string strToLower(string str)
{
	std::transform(str.begin(), str.end(), str.begin(),
    [](unsigned char c){ return std::tolower(c); });
	return str;
}

// https://stackoverflow.com/questions/1798112/removing-leading-and-trailing-spaces-from-a-string
string strip(const string& str, const string& whitespace)
{
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}
