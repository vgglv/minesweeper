#include "GameBoard.h"
#include "Constants.h"
#include "GameWindow.h"
#include <vector>
#include <memory>
#include <random>
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
		drawTile(tile.get());
		if (tile->type == TileState::FLAGGED) {
			_current_flags_count++;
		}
	}

	current_flags_count = _current_flags_count;
}

void GameBoard::drawTile(const Tile* tile) {
	Rectangle cell_rect = {static_cast<float>(tile->x * TILE_SIZE), static_cast<float>(tile->y * TILE_SIZE), TILE_SIZE, TILE_SIZE};
	switch (tile->type) {
		case TileState::SEALED:
		{
			DrawRectangleLinesEx(cell_rect, 1.f, WHITE);
			break;
		}
		case TileState::BOMB:
		{
			DrawRectangleRec(cell_rect, RED);
			DrawRectangleLinesEx(cell_rect, 1.f, WHITE);
			break;
		}
		case TileState::REVEALED:
		{
			const int x_offset = 10;
			DrawRectangleRec(cell_rect, GRAY);
			if (tile->count > 0) {
				drawNumber(tile);
			}
			DrawRectangleLinesEx(cell_rect, 1.f, WHITE);
			break;
		}
		case TileState::PRESSED:
		{
			DrawRectangleRec(cell_rect, GRAY);
			break;
		}
		case TileState::FLAGGED:
		{
			DrawRectangleRec(cell_rect, BLUE);
			DrawRectangleLinesEx(cell_rect, 1.f, WHITE);
			break;
		}
	}
}

void GameBoard::onLeftMouseRelease() {
	Tile* tile = findTileUnderCursor();
	if (tile == nullptr) {
		return;
	}
	if (tile->is_bomb) {
		GameWindow::setGameState(GameState::LOSE);
		tile->type = TileState::BOMB;
		return;
	}
	markTileRevealedRecursively(tile);
}

void GameBoard::onLeftMouseDown() {
	Tile* tile = findTileUnderCursor();
	if (tile == nullptr) {
		return;
	}
}

void GameBoard::onRightMouseRelease() {
	Tile* tile = findTileUnderCursor();
	if (tile == nullptr) {
		return;
	}
	if (tile->type == TileState::FLAGGED) {
		tile->type = TileState::SEALED;
		return;
	}
	if (tile->type != TileState::SEALED) {
		return;
	}
	tile->type = TileState::FLAGGED;
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
	tile->type = TileState::REVEALED;
	uint8_t mines_count = 0;
	static std::vector<std::pair<int, int>> relative_positions = {
		{-1, -1}, {0, -1}, {1, -1},
		{-1, 0},           {1, 0},
		{-1, 1},  {0, 1},  {1, 1}
	};
	for (const auto& [x, y] : relative_positions) {
		Tile* _tile = findTile(tile->x + x, tile->y + y);
		if (_tile == nullptr) {
			continue;
		}
		if (_tile->is_bomb) {
			mines_count++;
		}
	}
	if (mines_count == 0) {
		// no mines nearby, repeat the process
		for (const auto& [x, y] : relative_positions) {
			const int new_x = tile->x + x;
			const int new_y = tile->y + y;
			Tile* _tile = findTile(new_x, new_y);
			if (_tile == nullptr) {
				continue;
			}
			if (_tile->type != TileState::SEALED) {
				continue;
			}
			markTileRevealedRecursively(_tile);
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
			if (tile->is_bomb && tile->type == TileState::FLAGGED) {
				mines_n_flagged_count++;
			}
		}
	}

	if (mines_n_flagged_count == settings.mines_count) {
		GameWindow::setGameState(GameState::WIN);
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
			tile->type = TileState::FLAGGED;
		}
	}
	checkWinState();
}

void GameBoard::drawNumber(const Tile* tile) {
	const float x = static_cast<float>(tile->x) * static_cast<float>(TILE_SIZE);
	const float y = static_cast<float>(tile->y) * static_cast<float>(TILE_SIZE);
	const float tile_half = static_cast<float>(TILE_SIZE) / 2.F;
	const char* text = TextFormat("%d", tile->count);
	Vector2 text_size = MeasureTextEx(GetFontDefault(), text, TILE_SIZE, 1.F);
	const Vector2 text_pos = {x + tile_half - text_size.x / 2.F, y + tile_half - text_size.y / 2.F + 3.F};
	DrawTextEx(GetFontDefault(), text, text_pos, TILE_SIZE, 1.F, RAYWHITE);
}

void GameBoard::drawMenu() {
	const char* mines_text = TextFormat("MINES: %d", settings.mines_count);
	const float bottom_line_y = static_cast<float>(settings.board_height * TILE_SIZE) + 2.F;
	const Vector2 mines_text_pos = {TILE_SIZE, bottom_line_y};
	DrawTextEx(GetFontDefault(), mines_text, mines_text_pos, TILE_SIZE, 1.5F, RAYWHITE);

	const char* flags_text = TextFormat("FLAGS: %d", current_flags_count);
	const Vector2 flags_text_pos = {TILE_SIZE * 8.F, bottom_line_y};
	DrawTextEx(GetFontDefault(), flags_text, flags_text_pos, TILE_SIZE, 1.5F, RAYWHITE);
}
