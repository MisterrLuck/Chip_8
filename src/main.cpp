#include <iostream>

#include "graphics.hpp"
#include "chip8.hpp"
using std::cout;


int main()
{
	Chip8 chip8;
	Graphics graphics;

	if (!graphics.init())
	{
		cout << "Error with the graphics\n";
		return -1;
	}

	bool success = true;
	while (success)
	{
		success = graphics.updateFrame();
	}

	return 0;
}
