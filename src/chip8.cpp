#include "chip8.hpp"
#include "raylib.h"

#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <format>
#include <ctime>
#include <fstream>
#include <iterator>
#include <vector>
#include <cmath>

// using namespace std;
using std::cout, std::endl;

void Chip8::init()
{
	// Reset variables
	pc = 0x200;
	opcode = 0;
	I = 0;
	sp = 0;

	// Reset timers
	delay_timer = 0;
	sound_timer = 0;

	// Clear display
	clearScreen();
	
	// Clear stack
	size_t ind;
	for (ind = 0; ind < 16; ind++)
		stack[ind] = 0;

	// Clear registers V0-VF
	for (ind = 0; ind < 16; ind++)
		V[ind] = 0;

	// Clear memory
	for (ind = 0; ind < 4096; ind++)
		memory[ind] = 0;

	// Load fontset:
	for (ind = 0; ind < 80; ind++)
		memory[ind + 0x50] = chip8_fontset[ind];
}

// TODO: split into two functions; one for the debugger to read, and one for the emulator to fetch
// TODO: Also in the fetch; increment pc, and then decrement if needed in the instruction. Jumps wont need to decrement bc its already overwritten
int Chip8::getOpcode(int address) {
	if (address < 0)
		return memory[pc] << 8 | memory[pc + 1];
	return memory[address] << 8 | memory[address + 1];
}

void Chip8::emulateCycle()
{
	// TODO: Add reset button to reset the program as it never ends
	// TODO: Also a stop button obv
	
	// NOTE: Timing can vary; standard speed is 700 instructions per second
	// TODO: Slow down timing to 700 ins/sec

	// NOTE: memory[pc, pc+1] = 0xA520 : (0xA5 << 8) | 0x20 = 0xA520
	// TODO: Add error manager
	// with errors for accessing memory out of bounds
	opcode = getOpcode();

	int X = (opcode >> 8) & 0xF;
	int Y = (opcode >> 4) & 0xF;
	int N    = opcode & 0x000F;
	int NN   = opcode & 0x00FF;
	int NNN  = opcode & 0x0FFF;

	draw_flag = false;
	bool inc_pc = true;

	switch (opcode & 0xF000)
	{
		case 0x0000:
			switch (opcode & 0x0FFF)
			{
				case 0x00E0: // 0x00E0: Clears screen
					draw_flag = true;
					clearScreen();
				break;

				case 0x00EE: // 0x00EE: Return from subroutine
					// WARNING: Idk if that will work
					pc = stack[--sp]; // Should decrement and then access the stack
				break;
			}
		break;

		case 0x1000: // 0x1NNN : jump to address 
			pc = NNN;
			inc_pc = false;
		break;

		case 0x2000: // 0x2NNN : jump to subroutine at address
			stack[sp++] = pc; // save the current position, and increment sp
			pc = NNN;
			inc_pc = false;
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
				case 0x0000: // 0x8XY0 : store VY in VX
					V[X] = V[Y];
				break;

				case 0x0001: // 0x8XY1 : VX = VX OR VY
					if (OG_BOOLEAN_RESET)
						V[0xF] = 0;
					V[X] = V[X] | V[Y];
				break;

				case 0x0002: // 0x8XY2 : VX = VX AND VY
					if (OG_BOOLEAN_RESET)
						V[0xF] = 0;
					V[X] = V[X] & V[Y];
				break;

				case 0x0003: // 0x8XY3 : VX = VX XOR VY
					if (OG_BOOLEAN_RESET)
						V[0xF] = 0;
					V[X] = V[X] ^ V[Y];
				break;

				case 0x0004: // 0x8XY4 : VX = VX + VY with carry
				{
					int flag = 0;

					if (V[X] + V[Y] > 0xFF)
						flag = 1;

					V[X] += V[Y];
					V[0xF] = flag;
				}
				break;

				case 0x0005: // 0x8XY5 : VX - VY with underflow
				{
					int flag = 0;

					if (V[X] >= V[Y])
						flag = 1;

					V[X] -= V[Y];
					V[0xF] = flag;
				}
				break;

				case 0x0006: // 0x8XY6 : shift VX right one, move bit to VF
					if (OG_SHIFT)
					{
						int flag = V[Y] & 0b1;
						V[X] = V[Y] >> 1;
						V[0xF] = flag;
					} else
					{
						int flag = V[X] & 0b1;
						V[X] >>= 1;
						V[0xF] = flag;
					}
				break;

				case 0x0007: // 0x8XY7 : VY - VX with underflow
				{
					int flag = 0;

					if (V[X] <= V[Y])
						flag = 1;

					V[X] = V[Y] - V[X];
					V[0xF] = flag;
				}
				break;

				case 0x000E: // 0x8XYE : shift VX left one, move bit to VF
					if (OG_SHIFT)
					{
						int flag = (V[Y] & 0x80) >> 7; // leading bit is 1
						V[X] = V[Y] << 1;
						V[0xF] = flag;
					} else
					{
						int flag = (V[X] & 0x80) >> 7;
						V[X] <<= 1;
						V[0xF] = flag;
					}
				break;
			}
		break;

		case 0x9000: // 0x9XY0 : Skep next if VX != VY
			if (V[X] != V[Y])
				pc += 2;
		break;

		case 0xA000: // 0xANNN : sets I to address NNN
			I = NNN;
		break;

		case 0xB000: // 0xBNNN or 0xBXNN : jumps to NNN/XNN with an offset of V0/VX
			if (OG_JUMP_OFFSET)
				pc = NNN + V[0];
			else
				pc = NNN + V[X];
			inc_pc = false;
		break;

		case 0xC000: // 0xCXNN : sets VX to a random number between 0 and 255 ANDED with NN
			srand(time(0));
			V[X] = rand() & NN;
		break;

		case 0xD000: // 0xDXYN : draw a sprite at (X,Y)
		{
			draw_flag = true;

			// Sprite data is located at address I, N is the number of bytes / rows
			int x = V[X] % SCREEN_WIDTH;
			int y = V[Y] % SCREEN_HEIGHT;
			
			V[0xF] = 0; // For collision detection
			// The height of the sprite
			for (size_t row = 0; row < N; row++)
			{
				// On the bottom of the screen
				if (y + row >= SCREEN_HEIGHT)
					break;

				// Current byte of data
				int byte = memory[I + row];
				// each bit in byte
				for (size_t bit_ind = 0; bit_ind < 8; bit_ind++)
				{
					// On the edge of the screen
					if (x + bit_ind >= SCREEN_WIDTH)
						break;

					// Get the bits from left to right
					int pixel = (byte >> (7 - bit_ind)) & 0b1;
					size_t ind = (SCREEN_WIDTH * (y + row)) + (x + bit_ind);

					// WARNING: Idk if this can cause problems
					// If the bit has flipped; ie both values are 1
					if (gfx[ind] & pixel != 0)
						V[0xF] = 1;

					gfx[ind] ^= pixel;
				}
			}
		}
		break;

		// Key instructions
		case 0xE000:
			switch (opcode & 0x00FF)
			{
				case 0x009E: // EX9E : skips next instruction if key in VX is pressed
					// TODO: Improve this somehow I don't like this method
					if (IsKeyDown(keyFromInt(V[X])))
						pc += 2;
				break;

				case 0x00A1: // EXA1 : skips next instruction if key in VX is not pressed
					if (!IsKeyDown(keyFromInt(V[X])))
						pc += 2;
				break;
			}
		break;

		case 0xF000: //0xFX..
			switch (opcode & 0x00FF)
			{
				case 0x0007: // 0xFX07 : Store value of delay timer in VX
					V[X] = delay_timer;
				break;

				case 0x000A: // 0xFX0A : Wait until key is pressed
				{
					int key = keyPressed();
					// cout << "Key: " << key << "\n";
					if (key != -1)
						V[X] = key;
					else
						inc_pc = false; // wont increment instruction until its pressed
				}
				break;

				case 0x0015: // 0xFX15 : Set value of delay timer to VX
					delay_timer = V[X];
				break;

				case 0x0018: // 0xFX18 : Set value of sound timer to VX
					sound_timer = V[X];
				break;

				case 0x001E: // 0xFX1E : Add VX to index register I
					I = (I + V[X]) & 0x0FFF;
				break;

				case 0x0029: // 0xFX29 : Set I to the font address of the hexadecimal character in VX
					// 0x50 is the font start
					// VX & 0xF is the character
					// 5 bytes per character
					I = 0x50 + ((V[X] & 0xF) * 5);
				break;

				case 0x0033: // 0xFX33 : stores value in VX as BCD
					memory[I] = V[X] / 100;
					memory[I+1] = (V[X] / 10) % 10;
					memory[I+2] = (V[X] % 100) % 10;
				break;

				case 0x0055: // 0xFX55 : Loads registers 0-X inclusive in memory at I
					for (size_t ind = 0; ind <= X; ind++)
						memory[I + ind] = V[ind];

					if (OG_REG_MEMORY_INDEX)
						I = I + X + 1;
				break;

				case 0x0065: // 0xFX65 : Store the memory from I into registers 0-X
					for (size_t ind = 0; ind <= X; ind++)
						V[ind] = memory[I + ind];

					if (OG_REG_MEMORY_INDEX)
						I = I + X + 1;
				break;
			}
		break;
	}

#ifdef DEBUG
	cout << "[" << std::format("{:x}", pc) << "]: " << std::format("{:x}", opcode) << endl;
#endif

	if (inc_pc)
		pc += 2;
}

void Chip8::clearScreen()
{
	for (size_t ind = 0; ind < 64 * 32; ind++)
	{
		gfx[ind] = 0;
	}
}

// WARNING: I think there might be issues with leftover time if the counter is at 0, 
// and there is still leftover that doesn't make sense
void Chip8::updateTimers(int time_per_tick, int time_passed)
{
	int delay_time = time_passed + leftover_delay_time;
	int sound_time = time_passed + leftover_sound_time;

	// If enough for one clock tick has passed
	int delay_subtraction_amount = (int) (delay_time / time_per_tick);
	int sound_subtraction_amount = (int) (sound_time / time_per_tick);

	leftover_delay_time = delay_time % time_per_tick;
	leftover_sound_time = sound_time % time_per_tick;

	if (sound_subtraction_amount >= sound_timer)
	{
		sound_timer = 0;
		leftover_sound_time = 0;
	}
	else
	{
		sound_timer -= sound_subtraction_amount;
		// TODO: Add actual beeps to the sound timer
		cout << "\a";
	}

	delay_timer = std::max(0, delay_timer - delay_subtraction_amount);
	if (delay_timer == 0)
		leftover_delay_time = 0;
}

// WARNING: This will keep a queue of pressed keys I think
int Chip8::keyPressed()
{
	int key = GetKeyPressed();

	while (key != 0)
	{
		// If the key pressed is one from our list
		for (size_t val = 0; val <= 0xF; val++)
		{
			if (key == keyFromInt(val))
				return val;
		}
		key = GetKeyPressed();
	}

	return -1;
}

int Chip8::keyFromInt(char val)
{
	val = val & 0xF;
	switch(val)
	{
		// Row 1
		case 1:
			return KEY_ONE;
		break;

		case 2:
			return KEY_TWO;
		break;

		case 3:
			return KEY_THREE;
		break;

		case 0xc:
			return KEY_FOUR;
		break;

		// Row 2
		case 4:
			return KEY_Q;
		break;

		case 5:
			return KEY_W;
		break;

		case 6:
			return KEY_E;
		break;

		case 0xd:
			return KEY_R;
		break;

		// Row 3
		case 7:
			return KEY_A;
		break;

		case 8:
			return KEY_S;
		break;

		case 9:
			return KEY_D;
		break;

		case 0xe:
			return KEY_F;
		break;

		// Row 4
		case 0xa:
			return KEY_Z;
		break;

		case 0:
			return KEY_X;
		break;

		case 0xb:
			return KEY_C;
		break;

		case 0xf:
			return KEY_V;
		break;
	}
	return KEY_NULL;
}

bool Chip8::loadProgram(string program)
{
	std::ifstream input(program, std::ios::binary);
	if (!input.is_open())
	{
		std::cerr << "Unable to find program\n";
		return false;
	}

	std::vector<unsigned char> bytes(
		(std::istreambuf_iterator<char>(input)),
		(std::istreambuf_iterator<char>()));

	input.close();

	for(size_t ind = 0; ind < bytes.size(); ind++)
	{
		memory[ind + 0x200] = bytes[ind];
	}

	opcode = getOpcode();

	return true;
}
