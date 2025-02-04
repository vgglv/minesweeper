#include "GameBoard.h"
#include "draw_functions.h"
#include "types.h"

// std
#include <vector>
#include <memory>
#include <random>
#include <algorithm>

// dependency
#include "raylib.h"

using namespace game;

namespace {
	std::vector<std::unique_ptr<Tile>> tiles;
	BoardSettings settings;
	unsigned int current_flags_count = 0;
}

void GameBoard::initialize(const BoardSettings& _settings) {
	tiles.clear();
	settings = _settings;
	generateBoardTiles();
	// autoPlay();
}

void GameBoard::generateBoardTiles() {
	for (int i=0;i<settings.mines_count;i++) {
		placeOneMine();
	}
	for (int i=0;i<settings.board_width;i++) {
		for (int j=0;j<settings.board_height;j++) {
			if (findTile(i,j) != nullptr) {
				continue;
			}
			auto tile = std::make_unique<Tile>();
			tile->x = i;
			tile->y = j;
			tiles.emplace_back(std::move(tile));
		}
	}
}

void GameBoard::placeOneMine() {
	std::random_device dev_x;
	std::mt19937 rng_x(dev_x());
	std::uniform_int_distribution<std::mt19937::result_type> dist_x(0,settings.board_width-1);

	std::random_device dev_y;
	std::mt19937 rng_y(dev_y());
	std::uniform_int_distribution<std::mt19937::result_type> dist_y(0,settings.board_height-1);

	unsigned int x = dist_x(rng_x);
	unsigned int y = dist_y(rng_y);
	if (findTile(x, y) != nullptr) {
		// we hit the same place with mine, try to regenerate
		placeOneMine();
		return;
	}
	auto tile = std::make_unique<Tile>();
	tile->x = static_cast<int>(x);
	tile->y = static_cast<int>(y);
	tile->is_bomb = true;
	tiles.emplace_back(std::move(tile));
}

void GameBoard::draw() {
	unsigned int _current_flags_count = 0;
	for (const auto& tile : tiles) {
		draw_functions::drawTile(tile.get());
		if (tile->state == TileState::FLAGGED) {
			_current_flags_count++;
		}
		if (tile->state == TileState::PRESSED) {
			tile->state = TileState::SEALED;
		}
	}

	current_flags_count = _current_flags_count;

	draw_functions::drawBoardMenu(settings.mines_count, current_flags_count);
}

void GameBoard::onLeftMouseRelease() {
	Tile* tile = findTileUnderCursor();
	if (tile == nullptr) {
		return;
	}
	markTileRevealedRecursively(tile);
}

void GameBoard::onLeftMouseDown() {
	Tile* tile = findTileUnderCursor();
	if (tile == nullptr) {
		return;
	}
	if (tile->state != TileState::REVEALED) {
		return;
	}
	// toggle nearby sealed tiles as pressed
	static std::vector<std::pair<int, int>> relative_positions = {
		{-1, -1}, {0, -1}, {1, -1},
		{-1, 0}, {1, 0},
		{-1, 1}, {0, 1}, {1, 1}
	};
	for (const auto& [x, y] : relative_positions) {
		Tile* _tile = findTile(tile->x + x, tile->y + y);
		if (_tile == nullptr) {
			continue;
		}
		if (_tile->state != TileState::SEALED) {
			continue;
		}
		_tile->state = TileState::PRESSED;
	}
}

void GameBoard::onRightMouseRelease() {
	Tile* tile = findTileUnderCursor();
	if (tile == nullptr) {
		return;
	}
	if (tile->state == TileState::FLAGGED) {
		tile->state = TileState::SEALED;
		return;
	}
	if (tile->state == TileState::REVEALED) {
		// user wants to automate
		uint8_t flagged_count = 0;
		for (int dx = -1; dx <= 1; dx++) {
			for (int dy = -1; dy <= 1; dy++) {
				Tile* _tile = findTile(tile->x + dx, tile->y + dy);
				if (_tile == nullptr) {
					continue;
				}
				if (_tile->state == TileState::FLAGGED) {
					flagged_count++;
				}
			}
		}
		if (flagged_count == tile->count) {
			// reveal all tiles
			for (int dx = -1; dx <= 1; dx++) {
				for (int dy = -1; dy <= 1; dy++) {
					Tile* _tile = findTile(tile->x + dx, tile->y + dy);
					if (_tile == nullptr) {
						continue;
					}
					if (_tile->state != TileState::SEALED) {
						continue;
					}
					markTileRevealedRecursively(_tile);
					//if (_tile->is_bomb) {
					//	GameWindow::setGameState(GameState::LOSE);
					//	_tile->state = TileState::BOMB;
					//	continue;
					//}
					//_tile->state = TileState::REVEALED;
				}
			}
		}
		return;
	}
	if (tile->state != TileState::SEALED) {
		return;
	}
	tile->state = TileState::FLAGGED;
	checkWinState();
}

Tile* GameBoard::findTileUnderCursor() {
	const auto& it = std::find_if(tiles.begin(), tiles.end(), [](const std::unique_ptr<Tile>& tile) {
		const int& mouse_x = GetMouseX();
		const int& mouse_y = GetMouseY();
		return mouse_x >= tile->x * TILE_SIZE && 
			mouse_x < (tile->x + 1) * TILE_SIZE &&
			mouse_y >= tile->y * TILE_SIZE && 
			mouse_y < (tile->y + 1) * TILE_SIZE;
	});
	if (it != tiles.end()) {
		return it->get();
	}
	return nullptr;
}

Tile* GameBoard::findTile(int tile_x, int tile_y) {
	const auto& it = std::find_if(tiles.begin(), tiles.end(), [tile_x, tile_y](const std::unique_ptr<Tile>& tile) {
		return tile_x == tile->x && 
			tile_y == tile->y;
	});
	if (it != tiles.end()) {
		return it->get();
	}
	return nullptr;
}

void GameBoard::markTileRevealedRecursively(Tile* tile) {
	if (tile->is_bomb) {
		current_state = GameState::LOSE;
		tile->state = TileState::BOMB;
		return;
	}
	tile->state = TileState::REVEALED;
	uint8_t mines_count = 0;
	for (int dx = -1; dx <= 1; dx++) {
		for (int dy = -1; dy <= 1; dy++) {
			Tile* _tile = findTile(tile->x + dx, tile->y + dy);
			if (_tile == nullptr) {
				continue;
			}
			if (_tile->is_bomb) {
				mines_count++;
			}
		}
	}
	if (mines_count == 0) {
		// no mines nearby, repeat the process
		for (int dx = -1; dx <= 1; dx++) {
			for (int dy = -1; dy <= 1; dy++) {
				Tile* _tile = findTile(tile->x + dx, tile->y + dy);
				if (_tile == nullptr) {
					continue;
				}
				if (_tile->state != TileState::SEALED) {
					continue;
				}
				markTileRevealedRecursively(_tile);
			}
		}
	}
	tile->count = mines_count;
}

void GameBoard::checkWinState() {
	uint8_t mines_n_flagged_count = 0;
	for (int i=0;i<settings.board_width;i++) {
		for (int j=0;j<settings.board_height;j++) {
			const Tile* tile = findTile(i, j);
			if (tile == nullptr) {
				continue;
			}
			if (tile->is_bomb && tile->state == TileState::FLAGGED) {
				mines_n_flagged_count++;
			}
		}
	}

	if (mines_n_flagged_count == settings.mines_count) {
		current_state = GameState::WIN;
	}
}

uint8_t GameBoard::getBoardWidth() {
	return settings.board_width;
}

void GameBoard::autoPlay() {
	for (const auto& tile : tiles) {
		if (!tile->is_bomb) {
			markTileRevealedRecursively(tile.get());
		} else {
			tile->state = TileState::FLAGGED;
		}
	}
	checkWinState();
}
