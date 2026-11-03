#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <vector>

#include "CLI11.hpp"
#include "config.hpp"

#include "keyboard.hpp"
#include "debugger.hpp"
#include "sound.hpp"
#include "chip8.hpp"
#include "graphics.hpp"
using std::cout, std::string, std::cerr, std::vector;

void argsSettings(CLI::App* app, int argc, char *argv[]);


// Everything is run at 60Hz
const int FPS = 60;
const int MSPF = 1000000 / FPS; // Micro Seconds per Frame
int IPF = 11; // 15 with the wait quirk I think

// Argument Variables
string file_name = "";

bool debug_flag = false;
Config config = Config::CHIP_8;

vector<unsigned char> background = {0, 0, 0};
vector<unsigned char> foreground = {0xFF, 0xFF, 0xFF};
int scale = 20;


// TODO: Use raygui when combining the debugger such into one window (https://github.com/raysan5/raygui)
// TODO: Interesting chip 8 binary thing (https://github.com/Timendus/chip8-binary-format)
int main(int argc, char *argv[])
{
	SetTraceLogLevel(LOG_ERROR); 
	
	// Argument Parser
	CLI::App app{"Chip 8: Ver 2.2"};
	argsSettings(&app, argc, argv);
	CLI11_PARSE(app, argc, argv);

	Color background_color = (Color) {background[0], background[1], background[2], 255};
	Color foreground_color = (Color) {foreground[0], foreground[1], foreground[2], 255};

	// Set configuration
	setConfig(config);
	if (Quirk::drawWait)
		IPF = 15;

	// Initialization
	Chip8 chip8;
	GraphicsManager graphics_manager(scale, background_color, foreground_color);
	SoundManager sound_manager;
	Debugger debugger;

	// Load the Program
	bool loaded = chip8.loadProgram(file_name);

	if (!loaded)
		return -1;
	

	// Setup Debugger
	if (debug_flag)
	{
		debugger.init(&chip8);
		if (!debugger.prompt())
			return 0;
	}

	// Main Loop
	bool opened = true;
	while (!WindowShouldClose() && opened)
	{
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

		for (int i = 0; i < IPF; i++)
		{
			chip8.updateKeys(pollKeyboard());

			chip8.emulateCycle();


			if (debug_flag)
			{
				std::chrono::steady_clock::time_point midpoint = std::chrono::steady_clock::now();

				if (!debugger.prompt())
					return 0;

				// Offset the start time with how long the debugging prompt was
				begin += std::chrono::duration_cast<std::chrono::microseconds>(midpoint - std::chrono::steady_clock::now());
			}

			if ((chip8.draw_flag && Quirk::drawWait) || chip8.key_wait_flag)
				break;

			if (WindowShouldClose())
			{
				opened = false;
				break;
			}
		}

		graphics_manager.updateFrame(chip8.gfx);

		if (chip8.isSoundPlaying())
			sound_manager.playSound();
		else
			sound_manager.stopSound();

		chip8.decrementTimers();

		// Clamp to 60 Hz
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		int frame_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

		int remaining_frame_time = MSPF - frame_duration;

		if (remaining_frame_time > 5)
			std::this_thread::sleep_for(std::chrono::microseconds(remaining_frame_time));
	}
	
	return 0;
}


void argsSettings(CLI::App* app, int argc, char *argv[])
{
	argv = app->ensure_utf8(argv);

	app->add_option("file_path", file_name, "Program to run")
		->required();

	// Main Options
	app->add_flag("-d,--debug", debug_flag, "Run the program in debug");


	// Configuration Options
    app->add_option("-c,--config", config, "Configuration Version")
        ->transform(CLI::CheckedTransformer(CONFIG_CLI_MAP, CLI::ignore_case));


	// Extra Options
	app->add_option("--background", background, "Change the background color")
		->group("EXTRAS")
		->expected(3);

	app->add_option("--foreground", foreground, "Change the foreground color")
		->group("EXTRAS")
		->expected(3);

	app->add_option("--size,--scale", scale, "Change the pixel scale")
		->group("EXTRAS")
		->capture_default_str();
}
