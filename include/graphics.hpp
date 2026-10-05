#pragma once
#include "raylib.h"

class Graphics
{
public:
	void init(unsigned int scale = 20, Color off_color=BLACK, Color on_color=WHITE);

	void updateFrame(unsigned short *gfx);
	void drawPixel(int x, int y);

	~Graphics();

private:
	int scale;
	Color on, off;
};
