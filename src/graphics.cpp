#include "graphics.hpp"
#include "chip8.hpp"
#include "raylib.h"

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

		DrawText("Running Chip 8", 250, 20, 20, LIGHTGRAY);

	EndDrawing();

	return open && (!WindowShouldClose());
}

