#pragma once
#include <cstdint>

constexpr uint8_t TILE_SIZE = 32;
constexpr uint8_t BOARD_WIDTH = 30;
constexpr uint8_t BOARD_HEIGHT = 20;

enum class GameState {
	IDLE,
	WIN,
	LOSE,
	QUIT
};

enum class TileState {
	PRESSED,
	REVEALED,
	SEALED,
	BOMB,
	FLAGGED,
};

struct Tile {
	int x{};
	int y{};
	TileState state = TileState::SEALED;
	bool is_bomb = false;
	bool is_flagged = false;
	int count = 0;
};

static GameState current_state = GameState::IDLE;
