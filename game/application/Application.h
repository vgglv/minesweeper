#pragma once
#include <SDL.h>
#include <SDL_image.h>

namespace game::board {
	class Board;
}

namespace game::app {
	class Application {
	public:
		Application();
		~Application();

		bool initialize();
		void draw();
		bool isTerminating() const;
		void terminate();
		void run();

	private:
		bool is_terminating = false;
		game::board::Board* board = nullptr;
		SDL_Renderer* renderer = nullptr;
		SDL_Window* sdl_window = nullptr;
	};
}