#pragma once
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

#include <string>
using std::string;

#ifdef COSMIC
#define SHIFT
#define JUMP_OFFSET
#define FX1E_OVERFLOW
#define REG_MEMORY_INDEX
#endif

#ifdef SUPER_CHIP
#undef SHIFT
#undef JUMP_OFFSET
#undef FX1E_OVERFLOW
#undef REG_MEMORY_INDEX
#endif

class Chip8
{
public:
	bool draw_flag = false;

	void loadProgram(string program);

	void init();
	void emulateCycle();

	void clearScreen();

	int keyPressed();
	int keyFromInt(char val);

	void updateTimers(int time_per_tick, int time_passed);


	unsigned short opcode;

	// NOTE: Programs are loaded into address 0x200
	unsigned char memory[4096];
	unsigned short gfx[SCREEN_WIDTH * SCREEN_HEIGHT];

	unsigned char V[16];
	
	unsigned short stack[16];
	unsigned short sp;

	unsigned short I;
	unsigned short pc;

	// NOTE: The two timers: Both count down to 0 at 60Hz (60 times per second)
	// TODO: They should run in a separate thread as to be accurate
	unsigned char delay_timer;
	unsigned char sound_timer;

	// For the leftover timer microseconds for the next frame
	int leftover_time;

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

	// Configurations
#ifdef SHIFT
	bool OG_SHIFT = true;
#else
	bool OG_SHIFT = false;
#endif
#ifdef JUMP_OFFSET
	bool OG_JUMP_OFFSET = true;
#else
	bool OG_JUMP_OFFSET = false;
#endif
// TODO: Make better names for the configuration
#ifdef FX1E_OVERFLOW
	bool OG_FX1E_OVERFLOW = false;
#else
	bool OG_FX1E_OVERFLOW = true;
#endif
#ifdef REG_MEMORY_INDEX
	bool OG_REG_MEMORY_INDEX = true;
#else
	bool OG_REG_MEMORY_INDEX = false;
#endif
};
