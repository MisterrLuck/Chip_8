#include <iostream>
#include <unistd.h>
#include <string>

#include "graphics.hpp"
#include "chip8.hpp"
using std::cout, std::string;


int main(int argc, char *argv[])
{
	Chip8 chip8;
	Graphics graphics;

	// Initialization
	chip8.init();
	
	if (!graphics.init(20))
	{
		cout << "Error with the graphics\n";
		return -1;
	}

	// chip8.loadProgram("../roms/IBM_logo.ch8");
	chip8.loadProgram("../roms/test_opcode.ch8");
	// if (argc > 1)
	// 	chip8.loadProgram(string(argv[1]));
	// else
	// 	chip8.loadProgram("../roms/IBM_logo.ch8");

	bool running = true;
	while (running && (!WindowShouldClose()))
	{
		chip8.emulateCycle();
		
		if (chip8.draw_flag)
		{
			graphics.updateFrame(chip8.gfx);

			// Make sure the graphics line up with the array
			// Show graphics
			// size_t ind = 0;
			// for (size_t y = 0; y < SCREEN_HEIGHT; y++)
			// {
			//    	 for (size_t x = 0; x < SCREEN_WIDTH; x++)
			//    	 {
			//    			 ind = (SCREEN_WIDTH * y) + x;
			//    			 cout << chip8.gfx[ind];
			//    	 }
			//    	 cout << "\n";
			// }
			// cout << "ind: " << ind << "\n";
		}


		// sleep(1);
	}

	
	return 0;
}
