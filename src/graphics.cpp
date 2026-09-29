#include "graphics.hpp"
#include "chip8.hpp"
#include "raylib.h"
#include <cstddef>

Graphics::~Graphics()
{
    CloseWindow();
}

bool Graphics::init(unsigned int pixel_scale)
{
	if (pixel_scale <= 0)
		pixel_scale = 20;
	scale = pixel_scale;

    InitWindow(SCREEN_WIDTH * scale, SCREEN_HEIGHT * scale, "Chip 8");

    SetTargetFPS(60);

	open = true;
	return open;
}

bool Graphics::updateFrame(unsigned short *gfx)
{
	BeginDrawing();

		ClearBackground(RAYWHITE);

		for (size_t x = 0; x < SCREEN_WIDTH; x++)
		{
			for (size_t y = 0; y < SCREEN_HEIGHT; y++)
			{
				size_t ind = (SCREEN_WIDTH * y) + x;

				// WARNING: The only two numbers should be 1 and 0
				if (gfx[ind] == 1)
				{
					Vector2 pos = {(float) (x * scale), (float) (y * scale)};

					DrawRectangleV(pos, {(float) scale, (float) scale}, BLACK);
				}
			}
		}

	EndDrawing();

	return open;
}

