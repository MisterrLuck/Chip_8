#include <iostream>

#include "graphics.hpp"
#include "chip8.hpp"
using std::cout;


int main()
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

	chip8.loadProgram();

	// for (size_t i = 0; i < 132; i += 2)
	// {
	// 	cout << static_cast<int>(chip8.memory[0x200 + i]) << static_cast<int>(chip8.memory[0x200 + 1 + i]) << " ";
	//
	// 	if ((i+1) % 30 == 0)
	// 		cout << "\n";
	// }

	// while (true)
	// {
	// 	bool ret = chip8.emulateCycle();
	// 	// cout << "Return: " << ret << "\n";
	//
	// 	// ret = false;
	// 	if (ret)
	// 	{
	// 		// Show graphics
	// 		for (size_t y = 0; y < SCREEN_HEIGHT; y++)
	// 		{
	// 			for (size_t x = 0; x < SCREEN_WIDTH; x++)
	// 			{
	// 				size_t ind = (SCREEN_HEIGHT * y) + x;
	// 				cout << chip8.gfx[ind];
	// 			}
	// 			cout << "\n";
	// 		}
	// 	}
	// }

	bool running = true;
	unsigned short temp;
	while (running)
	{
		running = graphics.updateFrame(&temp);
	}

	
	return 0;
}
