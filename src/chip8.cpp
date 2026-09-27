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
	sp = 0x200;
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
	// memory[sp, sp+1] = 0xA520 : (0xA5 << 8) | 0x20 = 0xA520
	opcode = memory[sp] << 8 | memory[sp + 1];

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

	// if (delay_timer > 0)
	// 	delay_timer--;
	delay_timer = max(delay_timer-1, 0);
	if (sound_timer > 0)
	{
		if (sound_timer == 1)
			cout << "BEEP" << endl;
		--sound_timer; // Why before the var name?? it wouldnt matter in this context
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



// OPCODES

// []	0NNN		Execute machine language subroutine at address NNN
// [X]	00E0		Clear the screen
// [X]	00EE		Return from a subroutine
// []	1NNN		Jump to address NNN
// [X]	2NNN		Execute subroutine starting at address NNN
// [X]	3XNN		Skip the following instruction if the value of register VX equals NN
// [X]	4XNN		Skip the following instruction if the value of register VX is not equal to NN
// [X]	5XY0		Skip the following instruction if the value of register VX is equal to the value of register VY
// [X]	6XNN		Store number NN in register VX
// [X]	7XNN		Add the value NN to register VX
// []	8XY0		Store the value of register VY in register VX
// []	8XY1		Set VX to VX OR VY
// []	8XY2		Set VX to VX AND VY
// []	8XY3		Set VX to VX XOR VY
// [X]	8XY4		Add the value of register VY to register VX. Set VF to 01 if a carry occurs. Set VF to 00 if a carry does not occur
// []	8XY5		Subtract the value of register VY from register VX. Set VF to 00 if a borrow occurs. Set VF to 01 if a borrow does not occur
// []	8XY6		Store the value of register VY shifted right one bit in register VX. Set register VF to the least significant bit prior to the shift. VY is unchanged
// []	8XY7		Set register VX to the value of VY minus VX. Set VF to 00 if a borrow occurs. Set VF to 01 if a borrow does not occur
// []	8XYE		Store the value of register VY shifted left one bit in register VX Set register VF to the most significant bit prior to the shift VY is unchanged
// []	9XY0		Skip the following instruction if the value of register VX is not equal to the value of register VY
// [X]	ANNN		Store memory address NNN in register I
// []	BNNN		Jump to address NNN + V0
// []	CXNN		Set VX to a random number with a mask of NN
// [X]	DXYN		Draw a sprite at position VX, VY with N bytes of sprite data starting at the address stored in I Set VF to 01 if any set pixels are changed to unset, and 00 otherwise
// []	EX9E		Skip the following instruction if the key corresponding to the hex value currently stored in register VX is pressed
// []	EXA1		Skip the following instruction if the key corresponding to the hex value currently stored in register VX is not pressed
// []	FX07		Store the current value of the delay timer in register VX
// []	FX0A		Wait for a keypress and store the result in register VX
// []	FX15		Set the delay timer to the value of register VX
// []	FX18		Set the sound timer to the value of register VX
// []	FX1E		Add the value stored in register VX to register I
// []	FX29		Set I to the memory address of the sprite data corresponding to the hexadecimal digit stored in register VX
// [x]	FX33		Store the binary-coded decimal equivalent of the value stored in register VX at addresses I, I + 1, and I + 2
// []	FX55		Store the values of registers V0 to VX inclusive in memory starting at address I is set to I + X + 1 after operation²
// []	FX65		Fill registers V0 to VX inclusive with the values stored in memory starting at address I is set to I + X + 1 after operation
