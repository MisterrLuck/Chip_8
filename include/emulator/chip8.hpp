#pragma once
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

#include "config.hpp"

#include <string>
using std::string;


class Chip8
{
public:
	bool draw_flag = false;

	// TODO: Include sha hash thingy with metadata about the program (https://github.com/chip-8/chip-8-database)
	bool loadProgram(string program);

	void init(Config config);
	void setConfig(Config config);
	void emulateCycle();

	void clearScreen();

	int keyPressed();
	int keyFromInt(char val);

	void updateTimers(int time_per_tick, int time_passed);

	int getOpcode(int address=-1);

	unsigned short opcode;

	unsigned char memory[4096];
	unsigned short gfx[SCREEN_WIDTH * SCREEN_HEIGHT];

	unsigned char V[16];
	
	unsigned short stack[16];
	unsigned short sp;

	unsigned short I;
	unsigned short pc;

	unsigned char delay_timer;
	unsigned char sound_timer;

	// For the leftover timer microseconds for the next frame
	int leftover_sound_time;
	int leftover_delay_time;

	unsigned char chip8_fontset[80] =
	{
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};

	// NOTE: Configuration reference (https://chip-8.github.io/extensions/)
	// TODO: Add better changing configurations
	// TODO: Change names based on the 'standard' (https://github.com/chip-8/chip-8-database)

	// Configurations / Quirks
	bool OG_SHIFT;
	bool OG_JUMP_OFFSET;
	bool OG_REG_MEMORY_INDEX;
	bool OG_BOOLEAN_RESET;
};
