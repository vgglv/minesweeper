#pragma once
#include "Tile.h"
#include <vector>

namespace game::board {
	enum class eDifficulty {
		EASY
	};

	class Board {
	public:
		Board();
		~Board();

		bool initialize();

		void setBoardSize(unsigned int w, unsigned int h);
		void generateMines(eDifficulty difficulty);
		void draw(SDL_Renderer* renderer);

	private:
		std::vector<Tile*> tiles;
		unsigned int width = 0u;
		unsigned int height = 0u;

		bool initialized = false;
	};
}