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
	
	if (!graphics.init())
	{
		cout << "Error with the graphics\n";
		return -1;
	}

	bool running = true;
	while (running)
	{
		running = graphics.updateFrame();
	}

	return 0;
}
