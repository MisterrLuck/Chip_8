#pragma once
#include "raylib.h"

class GraphicsManager
{
public:
	GraphicsManager(unsigned int scale = 20, Color off=BLACK, Color on=WHITE);

	void updateFrame(unsigned short *gfx);
	void drawPixel(int x, int y);

	~GraphicsManager();

private:
	int scale;
	Color on_color, off_color;
};
