#pragma once
#include "raylib.h"

class Graphics
{
public:
	void init(unsigned int scale = 20, Color off=BLACK, Color on=WHITE);

	void updateFrame(unsigned short *gfx);
	void drawPixel(int x, int y);

	~Graphics();

private:
	int scale;
	Color on_color, off_color;
};
