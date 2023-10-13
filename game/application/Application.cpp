#include "Application.h"
#include <SDL.h>
#include <iostream>
#include "board/Board.h"
#include "loader/TextureLoader.h"

using namespace game::app;

namespace {
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
}

Application::Application() : board(new game::board::Board()) {

}

Application::~Application() {
	delete board;
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
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		std::cout << "Linear texture filtering not enabled!\n";
	}
	renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError()
			<< "\n";
		return false;
	}
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		std::cout << "SDL_image could not initialize!" <<
			"SDL_image Error: " << IMG_GetError() << "\n";
		return false;
	}
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	loader::TextureLoader::setRenderer(renderer);

	board->setBoardSize(40, 80);
	board->generateMines(board::eDifficulty::EASY);

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
		draw();
	}
}

void Application::draw() {
	SDL_RenderClear(renderer);

	board->draw(renderer);

	SDL_RenderPresent(renderer);
}

bool Application::isTerminating() const {
	return is_terminating;
}

void Application::terminate() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(sdl_window);

	IMG_Quit();
	SDL_Quit();
}