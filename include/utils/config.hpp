#pragma once

#include <string>
#include <map>
using std::string, std::map;

// NOTE: This is where to add more configs
enum class Config
{
	CHIP_8,
	CHIP_48
};

// NOTE: This is for the command line option
const map<string, Config> CONFIG_CLI_MAP{
	{"chip-8", Config::CHIP_8},
	{"chip-48", Config::CHIP_48}
};


// NOTE: Configuration reference (https://chip-8.github.io/extensions/)

// Quirks
namespace Quirk
{
	// On  : Opcodes `8XY6` and `8XYE` take `vX` as both input and output,
    // Off : Opcodes `8XY6` and `8XYE` take `vY` as input and `vX` as output
	static bool shiftOnlyVX = false;

	// On  : `FX55` and `FX65` increment the `i` register with `X`,
	// Off : `FX55` and `FX65` increment the `i` register with `X + 1`
	static bool memoryIncrementByX = false;

	// On  : `FX55` and `FX65` leave the `i` register unchanged,
	// Off : `FX55` and `FX65` increment the `i` register
	static bool memoryLeaveIUnchanged = false;

	// On  : The `DXYN` opcode wraps around to the other side of the screen when drawing at the edges,
	// Off : The `DXYN` opcode clips when drawing at the edges of the screen
	static bool drawWrap = false;

	// On  : Opcode `BXNN` jumps to address `XNN + vX`,
	// Off : Opcode `BNNN` jumps to address `NNN + v0`
	static bool jumpOffsetVX = false;

	// On  : Opcode `DXYN` waits for vertical blank (so max 60 sprites drawn per second),
	// Off : Opcode `DXYN` draws immediately (number of sprites drawn per second only limited to number of CPU cycles per frame)
	static bool drawWait = false; // TODO: To implement

	// On  : Opcodes `8XY1`, `8XY2` and `8XY3` (OR, AND and XOR) will set `vF` to zero after execution (even if `vF` is the parameter `X`),
	// Off : Opcodes `8XY1`, `8XY2` and `8XY3` (OR, AND and XOR) will leave `vF` unchanged (unless `vF` is the parameter `X`)
	static bool bitwiseResetVF = false;
}
