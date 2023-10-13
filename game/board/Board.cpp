#include "Board.h"

using namespace game::board;

Board::Board() {

}

Board::~Board() {
	for (auto tile : tiles) {
		delete tile;
	}
	tiles.clear();
}

bool Board::initialize() {
	if (width == 0u) {
		return false;
	}

	if (height == 0u) {
		return false;
	}

	return true;
}

void Board::setBoardSize(unsigned int w, unsigned int h) {
	width = w;
	height = h;
}

void Board::generateMines(eDifficulty difficulty) {
	for (int i=0; i < width; i++) {
		for (int j=0; j < height; j++) {
			auto tile = new Tile(i, j, eTileType::EMPTY);
			tiles.emplace_back(tile);
		}
	}
}

void Board::draw(SDL_Renderer* renderer) {
	for (const auto& tile : tiles) {
		tile->draw(renderer);
	}
}