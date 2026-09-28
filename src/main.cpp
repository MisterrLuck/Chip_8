#include <iostream>
// #include "../dependencies/GLFW/glfw3.h"
// #include <GLFW/glfw3.h>
#include "SDL.h"
#include "chip8.hpp"
using namespace std;

int main()
{
	Chip8 chip8;
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window *window = SDL_CreateWindow(
		"SDL2Test",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		480,
		0
	);

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	SDL_Delay(6000);

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
