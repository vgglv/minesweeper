#include "game.h"
#include <vector>
#include <random>
#include <raylib.h>
#include <algorithm>
#include <memory>
#include <iostream>
#include <variant>

using namespace game;


namespace {
	const uint8_t TILE_SIZE = 64;

	uint8_t mines_num = 10;

	std::vector<std::unique_ptr<Tile>> tiles;
	const uint8_t board_size_width = 12;
	const uint8_t board_size_height = 20;
	static bool NEED_TO_TERMINATE = false;
	static bool SHOW_WIN_TEXT = false;
}

void Game::initialize() {
	const int screen_width = board_size_width * TILE_SIZE;
	const int screen_height = board_size_height * TILE_SIZE + 1;
	std::cout << "Initializing screen: {" << screen_width <<
			", " << screen_height << "};\n";
	InitWindow(screen_width, screen_height, "Minesweeper");
	placeMines();
}

void Game::run() {
	while (!WindowShouldClose()) {
		if (NEED_TO_TERMINATE) {
			break;
		}
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			if (Tile* tile = findTileUnderCursor(GetMouseX(), GetMouseY())) {
				onTileLeftButtonDown(tile);
			}
		}

		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
			if (Tile* tile = findTileUnderCursor(GetMouseX(), GetMouseY())) {
				onTileLeftButtonRelease(tile);
			}
		}

		if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
			if (Tile* tile = findTileUnderCursor(GetMouseX(), GetMouseY())) {
				onTileRightButtonRelease(tile);
			}
		}
		BeginDrawing();
		ClearBackground(BLACK);
		for (const auto& tile : tiles) {
			drawTile(tile.get());
		}
		if (SHOW_WIN_TEXT) {
			const char* win_text = "You win!";
			Vector2 text_size = MeasureTextEx(GetFontDefault(), win_text, TILE_SIZE, 1.F);
			const int screen_width = board_size_width * TILE_SIZE;
			const Vector2 text_pos = { screen_width / 2.F - text_size.x / 2.F, 200.0 };
			Rectangle cell_rect = {text_pos.x - TILE_SIZE, text_pos.y - TILE_SIZE, text_size.x + TILE_SIZE * 2, text_size.y + TILE_SIZE * 2};
			DrawRectangleRec(cell_rect, BLUE);
			DrawRectangleLinesEx(cell_rect, 3.f, BLACK);
			DrawTextEx(GetFontDefault(), "You win!", text_pos, TILE_SIZE, 1.F, YELLOW);
		}
		EndDrawing();
	}
}

void Game::terminate() {
	CloseWindow();
}

void Game::placeMines() {
	for (int i=0;i<mines_num;i++) {
		std::random_device dev_x;
		std::mt19937 rng_x(dev_x());
		std::uniform_int_distribution<std::mt19937::result_type> dist_x(0,board_size_width-1);

		std::random_device dev_y;
		std::mt19937 rng_y(dev_y());
		std::uniform_int_distribution<std::mt19937::result_type> dist_y(0,board_size_height-1);

		unsigned int x = dist_x(rng_x);
		unsigned int y = dist_y(rng_y);
		auto tile = std::make_unique<Tile>();
		tile->x = static_cast<int>(x);
		tile->y = static_cast<int>(y);
		tile->is_bomb = true;
		std::cout << "Initializing Tile: {" << tile->x <<
				", " << tile->y << "};\n";
		tiles.emplace_back(std::move(tile));
	}
	for (int i=0;i<board_size_width;i++) {
		for (int j=0;j<board_size_height;j++) {
			if (findTile(i,j) != nullptr) {
				continue;
			}
			auto tile = std::make_unique<Tile>();
			tile->x = i;
			tile->y = j;
			std::cout << "Initializing Tile: {" << tile->x <<
					", " << tile->y << "};\n";
			tiles.emplace_back(std::move(tile));
		}
	}
}

void Game::drawTile(const Tile* tile) {
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
				const int TILE_OFFSET = 5;
				const int font_size = TILE_SIZE;
				const int x = static_cast<int>(tile->x) * static_cast<int>(TILE_SIZE) + x_offset;
				const int y = static_cast<int>(tile->y) * static_cast<int>(TILE_SIZE);
				DrawText(TextFormat("%d", tile->count), x, y, font_size, WHITE);
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

void Game::onTileLeftButtonRelease(Tile* tile) {
	if (tile->is_bomb) {
		// this is intentional, the program will crash because user clicked on mine >:D
		NEED_TO_TERMINATE = true;
		tile->type = TileState::BOMB;
		std::cout << "User clicked on bomb in {" << tile->x << ", " << tile->y << "};\n";
		return;
	}
	markTileRevealedRecursively(tile);
}

void Game::onTileRightButtonRelease(Tile* tile) {
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

void Game::onTileLeftButtonDown(Tile* tile) {
	//tile->type = TileState::PRESSED;
}

Tile* Game::findTileUnderCursor(unsigned int mouse_x, unsigned int mouse_y) {
	const auto& it = std::find_if(tiles.begin(), tiles.end(), [mouse_x, mouse_y](const std::unique_ptr<Tile>& tile) {
		return mouse_x >= tile->x * TILE_SIZE && mouse_x < (tile->x + 1) * TILE_SIZE &&
			mouse_y >= tile->y * TILE_SIZE && mouse_y < (tile->y + 1) * TILE_SIZE;
	});
	if (it != tiles.end()) {
		return it->get();
	}
	return nullptr;
}

Tile* Game::findTile(int tile_x, int tile_y) {
	const auto& it = std::find_if(tiles.begin(), tiles.end(), [tile_x, tile_y](const std::unique_ptr<Tile>& tile) {
		return tile_x == tile->x && tile_y == tile->y;
	});
	if (it != tiles.end()) {
		return it->get();
	}
	return nullptr;
}

void Game::markTileRevealedRecursively(Tile* tile) {
	std::cout << "Marking tile revealed on tile in {" << tile->x << ", " << tile->y << "};\n";
	tile->type = TileState::REVEALED;
	uint8_t mines_count = 0;
	static std::vector<std::pair<int, int>> relative_positions = {
		{-1, -1}, {0, -1}, {1, -1},
		{-1, 0},           {1, 0},
		{-1, 1},  {0, 1},  {1, 1}
	};
	for (const auto& [x, y] : relative_positions) {
		std::cout << "Performing search of a bomb...\n";
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

void Game::checkWinState() {
	uint8_t mines_n_flagged_count = 0;
	for (int i=0;i<board_size_width;i++) {
		for (int j=0;j<board_size_height;j++) {
			const Tile* tile = findTile(i, j);
			if (tile == nullptr) {
				continue;
			}
			if (tile->is_bomb && tile->type == TileState::FLAGGED) {
				mines_n_flagged_count++;
			}
		}
	}

	SHOW_WIN_TEXT = mines_n_flagged_count == mines_num;
}
