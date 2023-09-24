#include <Windows.h>
#include <iostream>
#include "SDL2/SDL.h"

namespace {
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
}

INT WINAPI WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_line, INT cmd_show) {

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL Error initializing, error: " << SDL_GetError() << "\n";
		return 1;
	}

	auto window = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!window) {
		std::cout << "SDL window is nullptr! Error: " << SDL_GetError() << "\n";
		return 1;
	}

	auto screen_surface = SDL_GetWindowSurface(window);

	SDL_FillRect(screen_surface, NULL, SDL_MapRGB(screen_surface->format, 0xFF, 0xFF, 0xFF));

	SDL_UpdateWindowSurface(window);

	SDL_Event e;
	bool terminating = false;
	while(!terminating) {
		while(SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				terminating = true;
				break;
			}
		}
	}

	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}