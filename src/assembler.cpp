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

	fout.write(binary_program.data(), binary_program.size());

	fout.close();
}

int Assembler::assembleInstruction(string instruction)
{
	single_byte = false;

	auto i = splitInstruction(instruction);
	instruction = get<0>(i);
	vector<string> parameters = get<1>(i);

	// TODO: Add to lower for the instruction and parameters
	// TODO: Add line number or address number probably 
	// TODO: Add SHL, SHR, and JUMPO
	// TODO: Move this around to check for each case exactly (param[0] is register && param[1] is NN), and then have general error if no cases
	if (instruction == "CLS")
	{
		if (parameters.size() != 0)
			throw std::invalid_argument("`CLS` instruction shouldn't have any parameters");
		return 0x00E0;
	}
	if (instruction == "RET")
	{
		if (parameters.size() != 0)
			throw std::invalid_argument("`RET` instruction shouldn't have any parameters");
		return 0x00EE;
	}
	if (instruction == "JUMP")
	{
		if (parameters.size() != 1)
			throw std::invalid_argument("`JUMP` instruction should only have one parameter");
		if (getLabelAddress(parameters[0]) == -1)
			throw std::invalid_argument(format("Label `{}` doesn't exist in `JUMP` instruction", parameters[0]));

		return 0x1000 | getLabelAddress(parameters[0]);
	}
	if (instruction == "CALL")
	{
		if (parameters.size() != 1)
			throw std::invalid_argument("`CALL` instruction should only have one parameter");
		if (getLabelAddress(parameters[0]) == -1)
			throw std::invalid_argument(format("Label `{}` doesn't exist in `CALL` instruction", parameters[0]));

		return 0x2000 | getLabelAddress(parameters[0]);
	}
	if (instruction == "SKE")
	{
		if (parameters.size() != 2)
			throw std::invalid_argument("`SKE` instruction should only have two parameters");
		if (getRegisterNum(parameters[0]) == -1)
			throw std::invalid_argument(format("First parameter `{}` in `SKE` has to be a register", parameters[0]));

		// Move the VX register into place
		int opcode = getRegisterNum(parameters[0]) << 8;
		if (getRegisterNum(parameters[1]) == -1) // NN as the second parameter
		{
			return 0x3000 | opcode | decodeNN(parameters[1]);
		}
		return 0x5000 | opcode | (getRegisterNum(parameters[1]) << 4);
	} if (instruction == "SKNE")
	{
		if (parameters.size() != 2)
			throw std::invalid_argument("`SKNE` instruction should only have two parameters");
		if (getRegisterNum(parameters[0]) == -1)
			throw std::invalid_argument(format("First parameter `{}` in `SKNE` has to be a register", parameters[0]));

		// Move the VX register into place
		int opcode = getRegisterNum(parameters[0]) << 8;
		if (getRegisterNum(parameters[1]) == -1)
		{
			return 0x4000 | opcode | decodeNN(parameters[1]);
		}
		return 0x9000 | opcode | (getRegisterNum(parameters[1]) << 4);
	} if (instruction == "STOR")
	{
		if (parameters[0] == "I")
		{
			if (getLabelAddress(parameters[1]) == -1)
				throw std::invalid_argument(format("Label `{}` doesn't exist in `STOR` instruction, after `I`", parameters[1]));

			return 0xA000 | getLabelAddress(parameters[1]);
		} if (parameters[0] == "DT")
		{
			if (getRegisterNum(parameters[1]) == -1)
				throw std::invalid_argument(format("Register `{}` doesn't exist in `STOR` instruction, after `DT`", parameters[1]));

			return 0xF015 | (getRegisterNum(parameters[1]) << 8);
		} if (parameters[0] == "ST")
		{
			if (getRegisterNum(parameters[1]) == -1)
				throw std::invalid_argument(format("Register `{}` doesn't exist in `STOR` instruction, after `ST`", parameters[1]));

			return 0xF018 | (getRegisterNum(parameters[1]) << 8);
		}
		else
		{
			if (parameters[1] == "DT")
			{
				if (getRegisterNum(parameters[0]) == -1)
					throw std::invalid_argument(format("Register `{}` doesn't exist in `STOR` instruction, before `DT`", parameters[0]));

				return 0xF007 | (getRegisterNum(parameters[0]) << 8);
			} if (getRegisterNum(parameters[1]) == -1)
			{
				if (getRegisterNum(parameters[0]) == -1)
					throw std::invalid_argument(format("Register `{}` doesn't exist in `STOR` instruction", parameters[0]));

				return 0x6000 | (getRegisterNum(parameters[0]) << 8) | decodeNN(parameters[1]);
			} if (getRegisterNum(parameters[0]) == -1)
				throw std::invalid_argument(format("Register `{}` doesn't exist in `STOR` instruction", parameters[0]));

			return 0x8000 | (getRegisterNum(parameters[0]) << 8) | (getRegisterNum(parameters[1]) << 4);
		}
	} if (instruction == "ADD")
	{
		if (parameters[0] == "I")
		{
			if (getRegisterNum(parameters[1]) == -1)
				throw std::invalid_argument(format("Register `{}` doesn't exist in `ADD` instruction", parameters[1]));

			return 0xF01E | (getRegisterNum(parameters[1]) << 8);
		} if (getRegisterNum(parameters[1]) == -1)
		{
			if (getRegisterNum(parameters[0]) == -1)
				throw std::invalid_argument(format("Register `{}` doesn't exist in `ADD` instruction", parameters[0]));

			return 0x7000 | (getRegisterNum(parameters[0]) << 8) | decodeNN(parameters[1]);
		} if (getRegisterNum(parameters[0]) == -1)
			throw std::invalid_argument(format("Register `{}` doesn't exist in `ADD` instruction", parameters[0]));

		return 0x8004 | (getRegisterNum(parameters[0]) << 8) | (getRegisterNum(parameters[1]) << 4);
	}  if (instruction == "OR")
	{
		if (getRegisterNum(parameters[0]) == -1)
			throw std::invalid_argument(format("Register `{}` doesn't exist in `OR` instruction", parameters[0]));
		if (getRegisterNum(parameters[1]) == -1)
			throw std::invalid_argument(format("Register `{}` doesn't exist in `OR` instruction", parameters[1]));

		return 0x8001 | (getRegisterNum(parameters[0]) << 8) | (getRegisterNum(parameters[1]) << 4);
	} if (instruction == "AND")
	{
		if (getRegisterNum(parameters[0]) == -1)
			throw std::invalid_argument(format("Register `{}` doesn't exist in `AND` instruction", parameters[0]));
		if (getRegisterNum(parameters[1]) == -1)
			throw std::invalid_argument(format("Register `{}` doesn't exist in `AND` instruction", parameters[1]));

		return 0x8002 | (getRegisterNum(parameters[0]) << 8) | (getRegisterNum(parameters[1]) << 4);
	} if (instruction == "XOR")
	{
		if (getRegisterNum(parameters[0]) == -1)
			throw std::invalid_argument(format("Register `{}` doesn't exist in `XOR` instruction", parameters[0]));
		if (getRegisterNum(parameters[1]) == -1)
			throw std::invalid_argument(format("Register `{}` doesn't exist in `XOR` instruction", parameters[1]));

		return 0x8003 | (getRegisterNum(parameters[0]) << 8) | (getRegisterNum(parameters[1]) << 4);
	} if (instruction == "SUBR")
	{
		if (getRegisterNum(parameters[0]) == -1)
			throw std::invalid_argument(format("Register `{}` doesn't exist in `SUBR` instruction", parameters[0]));
		if (getRegisterNum(parameters[1]) == -1)
			throw std::invalid_argument(format("Register `{}` doesn't exist in `SUBR` instruction", parameters[1]));

		return 0x8005 | (getRegisterNum(parameters[0]) << 8) | (getRegisterNum(parameters[1]) << 4);
	} if (instruction == "SUBL")
	{
		if (getRegisterNum(parameters[0]) == -1)
			throw std::invalid_argument(format("Register `{}` doesn't exist in `SUBL` instruction", parameters[0]));
		if (getRegisterNum(parameters[1]) == -1)
			throw std::invalid_argument(format("Register `{}` doesn't exist in `SUBL` instruction", parameters[1]));

		return 0x8007 | (getRegisterNum(parameters[0]) << 8) | (getRegisterNum(parameters[1]) << 4);
	}
	if (instruction == "RAND")
	{
		if (getRegisterNum(parameters[0]) == -1)
			throw std::invalid_argument(format("Register `{}` doesn't exist in `RAND` instruction", parameters[0]));

		return 0xC000 | (getRegisterNum(parameters[0]) << 8) | decodeNN(parameters[1]);
	}
	if (instruction == "DRAW")
	{
		if (getRegisterNum(parameters[0]) == -1)
			throw std::invalid_argument(format("Register `{}` doesn't exist in `DRAW` instruction", parameters[0]));
		if (getRegisterNum(parameters[1]) == -1)
			throw std::invalid_argument(format("Register `{}` doesn't exist in `DRAW` instruction", parameters[1]));

		return 0xD000 | (getRegisterNum(parameters[0]) << 8) | (getRegisterNum(parameters[1]) << 4) | decodeN(parameters[2]);
	}
	if (instruction == "SKK")
	{
		if (getRegisterNum(parameters[0]) == -1)
			throw std::invalid_argument(format("Register `{}` doesn't exist in `SKK` instruction", parameters[0]));

		return 0xE09E | (getRegisterNum(parameters[0]) << 8);
	}
	if (instruction == "SKNK")
	{
		if (getRegisterNum(parameters[0]) == -1)
			throw std::invalid_argument(format("Register `{}` doesn't exist in `SKNK` instruction", parameters[0]));

		return 0xE0A1 | (getRegisterNum(parameters[0]) << 8);
	}
	if (instruction == "KEY")
	{
		if (getRegisterNum(parameters[0]) == -1)
			throw std::invalid_argument(format("Register `{}` doesn't exist in `KEY` instruction", parameters[0]));

		return 0xF00A | (getRegisterNum(parameters[0]) << 8);
	}
	if (instruction == "CHAR")
	{
		if (getRegisterNum(parameters[0]) == -1)
			throw std::invalid_argument(format("Register `{}` doesn't exist in `CHAR` instruction", parameters[0]));

		return 0xF029 | (getRegisterNum(parameters[0]) << 8);
	}
	if (instruction == "BCD")
	{
		if (getRegisterNum(parameters[0]) == -1)
			throw std::invalid_argument(format("Register `{}` doesn't exist in `BCD` instruction", parameters[0]));

		return 0xF033 | (getRegisterNum(parameters[0]) << 8);
	}
	if (instruction == "SMEM")
	{
		if (getRegisterNum(parameters[0]) == -1)
			throw std::invalid_argument(format("Register `{}` doesn't exist in `SMEM` instruction", parameters[0]));

		return 0xF055 | (getRegisterNum(parameters[0]) << 8);
	}
	if (instruction == "LMEM")
	{
		if (getRegisterNum(parameters[0]) == -1)
			throw std::invalid_argument(format("Register `{}` doesn't exist in `LMEM` instruction", parameters[0]));

		return 0xF065 | (getRegisterNum(parameters[0]) << 8);
	}
	if (instruction == "BYTES")
	{
		// HACK:
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

	return -1;
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

// TODO: probably don't have the error thing inside this function bc now idk the instruction being run
// TODO: Add back the try catch statement to add better readability for the errors that might throw
int Assembler::decodeNN(string num)
{
	unsigned int n;
	size_t len = 0;

	if (num.substr(0, 2) == "0x")
		n = std::stoul(num, &len, 16);
	else if (num.substr(0, 2) == "0b")
		n = std::stoul(num, &len, 2);
	else
		n = std::stoul(num, &len, 10);

	if (len != num.size())
		throw std::invalid_argument(format("`{}` cannot be decoded", num));

	if (n > 0xFF)
		throw std::invalid_argument(format("`{}` cannot be over 1 byte", num));

	if (n < 0)
		throw std::invalid_argument(format("`{}` cannot be negative", num));

	return n;
}

int Assembler::decodeN(string num)
{
	unsigned int n;
	size_t len = 0;

	if (num.substr(0, 2) == "0x")
		n = std::stoul(num, &len, 16);
	else if (num.substr(0, 2) == "0b")
		n = std::stoul(num, &len, 2);
	else
		n = std::stoul(num, &len, 10);

	if (len != num.size())
		throw std::invalid_argument(format("`{}` cannot be decoded", num));

	if (n > 0xF)
		throw std::invalid_argument(format("`{}` cannot be over 4 bits", num));

	if (n < 0)
		throw std::invalid_argument(format("`{}` cannot be negative", num));

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
