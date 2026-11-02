#include "graphics.hpp"
#include "chip8.hpp"
#include "raylib.h"
#include <cstddef>

GraphicsManager::~GraphicsManager()
{
    CloseWindow();
}

GraphicsManager::GraphicsManager(unsigned int pixel_scale, Color off, Color on)
{
	off_color = off;
	on_color = on;

	if (pixel_scale <= 0)
		pixel_scale = 20;
	scale = pixel_scale;

    InitWindow(SCREEN_WIDTH * scale, SCREEN_HEIGHT * scale, "Chip 8");

    SetTargetFPS(60);
}

void GraphicsManager::updateFrame(unsigned short *gfx)
{
	BeginDrawing();

		ClearBackground(off_color);

		// TODO: Add redrawing only for the changed pixels; remember to remove the clear background then
		for (size_t x = 0; x < SCREEN_WIDTH; x++)
		{
			for (size_t y = 0; y < SCREEN_HEIGHT; y++)
			{
				size_t ind = (SCREEN_WIDTH * y) + x;

				// WARNING: The only two numbers should be 1 and 0
				if (gfx[ind] == 1)
				{
					Vector2 pos = {(float) (x * scale), (float) (y * scale)};

					DrawRectangleV(pos, {(float) scale, (float) scale}, on_color);
				}
			}
		}

	EndDrawing();
}

