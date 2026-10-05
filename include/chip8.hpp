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
	// I think this is flagged if the screen needs a redraw
	bool draw_flag = false;

	void loadProgram(string program);

	void init();
	void emulateCycle();

	void clearScreen();

	int keyPressed();
	int keyFromInt(char val);


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

	
	// TODO: Perhaps change this to a set of ints to pack, or even bools for easier use
	unsigned short gfx[SCREEN_WIDTH * SCREEN_HEIGHT];		// Display in monochrome; ie black and white
// private:

	unsigned short opcode;			// The current opcode

	// NOTE: Old programs expect to be loaded into address 0x200
	unsigned char memory[4096];		// This is where the program sits; RAM

	unsigned char V[16];			// The registers: VF isn't recommended for use, as it is also a flag
	
	unsigned short stack[16];		// Stack; only for returning from subroutines; limited to 16 2-byte entries
	unsigned short sp;				// Stack pointer


	// NOTE: These are technically 16 bits but are used as 12 bits ?
	unsigned short I;				// Index register used to point at locations in memory
	unsigned short pc;				// Program counter

	// NOTE: The two timers: Both count down to 0 at 60Hz (60 times per second)
	// They should run in a separate thread as to be accurate
	unsigned char delay_timer;		// Intended for timing in games; can be set and read
	unsigned char sound_timer;		// Will beep when its value is non-zero; can only be set

	// This is stored in the memory in the initialise function
	// NOTE: Stored at 0x50 with 5 byte increments
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

};

// https://multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/
// https://tobiasvl.github.io/blog/write-a-chip-8-emulator/

// 0x000-0x1FF - Chip 8 interpreter
// 0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
// 0x200-0xFFF - Program ROM and work RAM

// OPCODES

// NNN	->	Address
// NN	->	8-bit constant
// N	->	4-bit constant
// X,Y	->	4-bit register indentifiers
// PC	->	Program counter
// I	->	12-bit register? (for memory address)
// VN	->	One of the 16 registers: N from 0x0 to 0xF

// All are two bytes long, stored in big endian

// [X]	00E0		Clear the screen
// [X]	00EE		Return from a subroutine
// [X]	1NNN		Jump to address NNN
// [X]	2NNN		Execute subroutine starting at address NNN
// [X]	3XNN		Skip the following instruction if the value of register VX equals NN
// [X]	4XNN		Skip the following instruction if the value of register VX is not equal to NN
// [X]	5XY0		Skip the following instruction if the value of register VX is equal to the value of register VY
// [X]	6XNN		Store number NN in register VX
// [X]	7XNN		Add the value NN to register VX
// [X]	8XY0		Store the value of register VY in register VX
// [X]	8XY1		Set VX to VX OR VY
// [X]	8XY2		Set VX to VX AND VY
// [X]	8XY3		Set VX to VX XOR VY
// [X]	8XY4		Add the value of register VY to register VX. Set VF to 01 if a carry occurs. Set VF to 00 if a carry does not occur
// [X]	8XY5		Subtract the value of register VY from register VX. Set VF to 00 if a borrow occurs. Set VF to 01 if a borrow does not occur
// [X]	8XY6		Store the value of register VY shifted right one bit in register VX. Set register VF to the least significant bit prior to the shift. VY is unchanged
// [X]	8XY7		Set register VX to the value of VY minus VX. Set VF to 00 if a borrow occurs. Set VF to 01 if a borrow does not occur
// [X]	8XYE		Store the value of register VY shifted left one bit in register VX Set register VF to the most significant bit prior to the shift VY is unchanged
// [X]	9XY0		Skip the following instruction if the value of register VX is not equal to the value of register VY
// [X]	ANNN		Store memory address NNN in register I
// [X]	BNNN		Jump to address NNN + V0
// [X]	CXNN		Set VX to a random number with a mask of NN
// [X]	DXYN		Draw a sprite at position VX, VY with N bytes of sprite data starting at the address stored in I. Set VF to 01 if any set pixels are changed to unset, and 00 otherwise. Sprites are drawn as an XOR where it will flip the bit if the sprite has a one.
// [X]	EX9E		Skip the following instruction if the key corresponding to the hex value currently stored in register VX is pressed
// [X]	EXA1		Skip the following instruction if the key corresponding to the hex value currently stored in register VX is not pressed
// [X]	FX07		Store the current value of the delay timer in register VX
// [X]	FX0A		Wait for a keypress and store the result in register VX
// [X]	FX15		Set the delay timer to the value of register VX
// [X]	FX18		Set the sound timer to the value of register VX
// [X]	FX1E		Add the value stored in register VX to register I
// [X]	FX29		Set I to the memory address of the sprite data corresponding to the hexadecimal digit stored in register VX
// [X]	FX33		Store the binary-coded decimal equivalent of the value stored in register VX at addresses I, I + 1, and I + 2
// [X]	FX55		Store the values of registers V0 to VX inclusive in memory starting at address I is set to I + X + 1 after operation
// [X]	FX65		Fill registers V0 to VX inclusive with the values stored in memory starting at address I is set to I + X + 1 after operation

