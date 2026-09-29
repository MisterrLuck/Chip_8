#pragma once
// #include <GLFW/glfw3.h>
#include "raylib.h"

class Graphics
{
public:
	bool init(unsigned int scale = 20);

	bool updateFrame(unsigned short *gfx);
	void drawPixel(int x, int y);

	~Graphics();

private:
	bool open = false;
	int scale;
};
