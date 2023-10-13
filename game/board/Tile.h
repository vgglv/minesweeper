#pragma once
#include <SDL_image.h>

namespace game::board {
	enum class eTileType {
		NUMBER,
		MINE,
		EMPTY
	};

	class Tile {
	public:
		Tile(int x, int y, eTileType type);
		~Tile();

		eTileType type = eTileType::EMPTY;
		int x = 0;
		int y = 0;

		void draw(SDL_Renderer* renderer);

	private:
		SDL_Texture* texture = nullptr;
	};
}