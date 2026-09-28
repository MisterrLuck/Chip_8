#include "chip8.hpp"

#include <concepts>
#include <cstdio>
#include <fstream>
#include <ios>
#include <iosfwd>
#include <iostream>
using namespace std;

void Chip8::initialize()
{
	pc = 0x200;
	opcode = 0;
	I = 0;
	sp = 0;

	// Clear display
	// Clear stack
	// Clear registers V0-VF
	// Clear memory

	// Load fontset: ?what does this mean
	for (int i = 0; i < 80; i++)
		memory[i] = chip8_fontset[i];

	// Reset timers

	// Load the program into memory, perhaps ?
}

void Chip8::emulateCycle()
{
	// NOTE: Steps of the program
	// Fetch the current command
	// Decode to find out what to do
	// Execute the instruction
	
	// NOTE: Timing can vary; standard speed is 700 instructions per second

	// NOTE: memory[sp, sp+1] = 0xA520 : (0xA5 << 8) | 0x20 = 0xA520
	opcode = memory[sp] << 8 | memory[sp + 1];
	sp++;

	// Define them ahead of time in case the opcode uses them
	// Look up value in register
	int x;
	int y;
	// Hardcoded values
	int nnn;

	// get most significant byte : 0x(A)520
	switch (opcode & 0xF000)
	{
		case 0x0000: // if the most significant isn't enough
			switch (opcode & 0x000F)
			{
				case 0x0000: // 0x00E0: Clears screen

				break;

				case 0x000E: // 0x00EE: Return from subroutine

				break;

				default: // probably 0x0NNN
					cout << "Unknown opcode [0x0000]: " << opcode << endl;
			}
		break;

		case 0x2000: // 0x2NNN : jump to subroutine at address
			stack[sp++] = pc; // save the current position, and increment sp
			// sp++;
			pc = opcode & 0x0FFF;
		break;

		case 0x3000: // 0x3XNN : skip next instruction if VX == NN
			if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
				pc += 2;
			pc += 2;
		break;

		case 0x4000: // 0x4XNN : skip next instruction if VX != NN
			if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
				pc += 2;
			pc += 2;
		break;

		case 0x5000: // 0x5XYN : skip next instruction if VX == VY
			if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
				pc += 2;
			pc += 2;
		break;

		case 0x6000: // 0x6XNN : sets VX = NN
			V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
			pc += 2;
			break;

		case 0x7000: // 0x7XNN : adds value NN to register VX
			V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
			pc += 2;
		break;

		case 0x8000:
			switch (opcode & 0x000F)
			{
				case 0x0004: // 0x8XY4 : adds register value Y to X
					// if the adding number is more than the remaining amount in the other number, its a carry
					if (V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00)>> 8]))
						V[0xF] = 1; // carry bit
					else
						V[0xF] = 0;
					// Adds the two values and stores into X
					V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
					pc += 2;
				break;
			}
		break;

		case 0xA000: // 0xANNN : sets I to address NNN
			I = opcode & 0x0FFF; // get operands from opcode
			pc += 2; // increment two bytes in the program counter
		break;

		case 0xD000: // 0xDXYN : draw a sprite at (X,Y)
		{
			// Sprite data is located at address I, N is the number of bytes
			unsigned short x = V[(opcode & 0x0F00) >> 8];
			unsigned short y = V[(opcode & 0x00F0) >> 4];
			unsigned short height = opcode & 0x000F;
			unsigned short pixel;

			// for collision detection or smth
			V[0xF] = 0;
			for (int yline = 0; yline < height; yline++)
			{
				pixel = memory[I + yline];
				for (int xline = 0; xline < 8; xline++)
				{
					if ((pixel & (0x80 >> xline)) != 0)
					{
						// get the pixel position in the array
						if (gfx[(x + xline + ((y + yline ) * 64))] == 1)
							V[0xF] = 1;
						gfx[(x + xline + ((y + yline ) * 64))] ^= 1;
					}
				}
			}
			pc += 2;
		}
		break;

		case 0xE000:
			switch (opcode & 0x00FF)
			{
				case 0x009E: // EX9E : skips next instruction if key in VX is pressed
					if (key[V[(opcode & 0x0F00) >> 8]] != 0)
						pc += 4;
					else
						pc += 2;
				break;
			}
		break;

		// make more opcodes
		case 0xF000: //0xFX..
			switch (opcode & 0x00FF)
			{
				case 0x0033: // 0xFX33 : stores value in X as BCD
					memory[I] = V[(opcode & 0x0F00) >> 8] / 100;
					memory[I+1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
					memory[I+2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
					pc += 2;
				break;
			}
		break;

		default:
			cout << "Unknown opcode: " << opcode << endl;
	}
}

void Chip8::loadProgram()
{
	// fstream file("game.c8", ios::in | ios::binary);
	// if (file.is_open())
	// 	file.read()
	int bufferSize = 10; // Idk where to get this?
	unsigned char buffer[10]; // Idk about this either

	for(int i = 0; i < bufferSize; ++i)
		memory[i + 512] = buffer[i];
}
