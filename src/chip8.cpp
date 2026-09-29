#include "chip8.hpp"

#include <concepts>
#include <cstdio>
#include <fstream>
#include <ios>
#include <iosfwd>
#include <iostream>
using namespace std;

void Chip8::init()
{
	pc = 0x200;
	opcode = 0;
	I = 0;
	sp = 0;

	// Clear display
	clearScreen();
	// Clear stack
	// Clear registers V0-VF
	// Clear memory

	// Load fontset:
	for (int i = 0; i < 80; i++)
		memory[i+0x50] = chip8_fontset[i];

	// Reset timers
	delay_timer = 0;
	sound_timer = 0;

	// Load the program into memory
	loadProgram();
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
	int X_ind = (opcode & 0x0F00) >> 8;
	int Y_ind = (opcode & 0x00F0) >> 4;
	int X    = V[X_ind];
	int Y    = V[Y_ind];

	// Hardcoded values
	// NOTE: Should they be proper datatypes instead of ints ?
	int N    = opcode & 0x000F;
	int NN   = opcode & 0x00FF;
	int NNN  = opcode & 0x0FFF;

	// get most significant byte : 0x(A)520
	switch (opcode & 0xF000)
	{
		case 0x0000: // if the most significant isn't enough
			switch (opcode & 0x0FFF)
			{
				case 0x00E0: // 0x00E0: Clears screen
					clearScreen();
				break;

				case 0x00EE: // 0x00EE: Return from subroutine

				break;

				default: // probably 0x0NNN
					cout << "Unknown opcode [0x0000]: " << opcode << endl;
			}
		break;

		case 0x1000: // 0x1NNN : jump to address; without return address?
			pc = NNN;
		break;

		case 0x2000: // 0x2NNN : jump to subroutine at address
			stack[sp++] = pc; // save the current position, and increment sp
			pc = NNN;
		break;

		case 0x3000: // 0x3XNN : skip next instruction if VX == NN
			if (V[X] == NN)
				pc += 2;
		break;

		case 0x4000: // 0x4XNN : skip next instruction if VX != NN
			if (V[X] != NN)
				pc += 2;
		break;

		case 0x5000: // 0x5XY0 : skip next instruction if VX == VY
			if (V[X] == V[Y])
				pc += 2;
		break;

		case 0x6000: // 0x6XNN : sets VX = NN
			V[X] = NN;
		break;

		case 0x7000: // 0x7XNN : adds value NN to register VX
			V[X] += NN;
		break;

		case 0x8000:
			switch (opcode & 0x000F)
			{
				case 0x0004: // 0x8XY4 : adds register value Y to X
					// if the adding number is more than the remaining amount in the other number, its a carry
					// if (V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00)>> 8]))
					// 	V[0xF] = 1; // carry bit
					// else
					// 	V[0xF] = 0;
					// // Adds the two values and stores into X
					// V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
				break;
			}
		break;

		case 0xA000: // 0xANNN : sets I to address NNN
			I = NNN; // get operands from opcode
		break;

		case 0xD000: // 0xDXYN : draw a sprite at (X,Y)
		{
			// Sprite data is located at address I, N is the number of bytes / rows
			X = X % SCREEN_WIDTH;
			Y = Y % SCREEN_HEIGHT;
			
			V[0xF] = 0; // For collision detection
			// The height of the sprite
			for (size_t row = 0; row < N; row++)
			{
				// On the bottom of the screen
				if (Y + row >= SCREEN_HEIGHT)
					break;

				// Current byte of data
				int byte = memory[I + row];
				// each bit in byte
				for (size_t bit_ind = 0; bit_ind < 8; bit_ind++)
				{
					// On the edge of the screen
					if (X + bit_ind >= SCREEN_WIDTH)
						break;

					// Get the bits from left to right
					int pixel = (byte >> (7 - bit_ind)) & 0b1;

					size_t ind = (SCREEN_HEIGHT * (X + bit_ind)) + (Y + row);

					// WARNING: Idk if this can cause problems
					// If the bit has flipped; ie both values are 1
					if (gfx[ind] & pixel != 0)
						V[0xF] = 1;

					gfx[ind] ^= pixel;
				}
			}
		}
		break;

		case 0xE000:
			switch (opcode & 0x00FF)
			{
				case 0x009E: // EX9E : skips next instruction if key in VX is pressed
					// if (key[V[X]] != 0)
					// 	pc += 2;
				break;
			}
		break;

		case 0xF000: //0xFX..
			switch (opcode & 0x00FF)
			{
				case 0x0033: // 0xFX33 : stores value in X as BCD
					// memory[I] = V[X] / 100;
					// memory[I+1] = (V[X] / 10) % 10;
					// memory[I+2] = (V[X] % 100) % 10;
				break;
			}
		break;

		default:
			cout << "Unknown opcode: " << opcode << endl;
	}

	// Make sure to update the screen based on the graphics array
}

void Chip8::clearScreen()
{
	for (size_t ind = 0; ind < 64 * 32; ind++)
	{
		gfx[ind] = 0;
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
		memory[i + 0x200] = buffer[i];
}
