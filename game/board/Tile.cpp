#include "Tile.h"
#include "loader/TextureLoader.h"

using namespace game::board;

Tile::Tile(int x, int y, eTileType type) : x(x), y(y), type(type) {
	switch (type) {
	case eTileType::EMPTY:
		texture = loader::TextureLoader::loadTexture("assets/n0.png");
		break;
	case eTileType::MINE:
		texture = loader::TextureLoader::loadTexture("assets/mine.png");
		break;
	case eTileType::NUMBER:
		break;
	}
}

Tile::~Tile() {
	if (texture) {
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}
}

void Tile::draw(SDL_Renderer* renderer) {
	SDL_RenderCopy(renderer, texture, nullptr, nullptr);
}