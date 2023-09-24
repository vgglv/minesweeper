#include "application/Application.h"
#include <SDL.h>
#include <iostream>
#include "random/SeedGenerator.h"

using namespace game::app;

namespace {
	SDL_Window* sdl_window = nullptr;
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
}

bool Application::initialize() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL Error initializing, error: " << SDL_GetError() << "\n";
		return false;
	}

	sdl_window = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!sdl_window) {
		std::cout << "SDL window is nullptr! Error: " << SDL_GetError() << "\n";
		return false;
	}
	auto screen_surface = SDL_GetWindowSurface(sdl_window);

	SDL_FillRect(screen_surface, NULL, SDL_MapRGB(screen_surface->format, 0xFF, 0xFF, 0xFF));

	SDL_UpdateWindowSurface(sdl_window);

	auto seed = random::SeedGenerator::generateRandomSeed(5u);
	std::cout << "Generated seed: ";
	for (const auto& g : seed.getGeneratedSeedVector()) {
		std::cout << g;
	}
	std::cout << "\n";

	return true;
}

void Application::run() {
	SDL_Event e;
	while(!is_terminating) {
		while(SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				is_terminating = true;
				break;
			}
		}
		update();
	}
}

void Application::update() {
}

bool Application::isTerminating() const {
	return is_terminating;
}

void Application::terminate() {
	SDL_DestroyWindow(sdl_window);

	SDL_Quit();
}