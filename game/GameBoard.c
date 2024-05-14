#include "GameBoard.h"
#include "Constants.h"
#include "GameWindow.h"
#include "Tile.h"
#include "Painter.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

// dependency
#include "raylib.h"

Tile tiles[BOARD_WIDTH][BOARD_HEIGHT];
uint8_t _mines_count;
unsigned int current_flags_count = 0;

static void placeOneMine();
static void markTileRevealedRecursively(Tile* tile);
static void checkWinState();
static Tile* findTileUnderCursor();
static Tile* findTile(int tile_x, int tile_y);
static void generateBoardTiles();
static Tile generateTile(int x, int y) {
	struct Tile tile = {
		.x = x,
		.y = y,
		.draw_state = SEALED,
		.is_bomb = false,
		.is_flagged = false,
		.count = 0
	};
	return tile;
};

void GameBoard_initialize(const uint8_t mines_count) {
	printf("Initializing GameBoard\n");
	_mines_count = mines_count;
	generateBoardTiles();
}

static void generateBoardTiles() {
	for (int i=0;i<BOARD_WIDTH;i++) {
		for (int j=0;j<BOARD_HEIGHT;j++) {
			tiles[i][j] = generateTile(i, j);
		}
	}
	for (int i=0;i<_mines_count;i++) {
		placeOneMine();
	}
}

static void regenerateSeed() {
	static unsigned int seed = 0;
	if (seed == 0) {
		seed = (unsigned int)time(NULL);
	}
	seed++;
	srand(seed);
}

static void placeOneMine() {
	regenerateSeed();
	unsigned int x = rand() % (BOARD_WIDTH - 1);
	regenerateSeed();
	unsigned int y = rand() % (BOARD_HEIGHT - 1);

	if (tiles[x][y].is_bomb == true) {
		placeOneMine();
		return;
	}

	tiles[x][y].is_bomb = true;
}

void GameBoard_draw() {
	unsigned int _current_flags_count = 0;
	for (int i=0;i<BOARD_WIDTH;i++) {
		for (int j=0;j<BOARD_HEIGHT;j++) {
			Tile* tile_pointer = &tiles[i][j];
			Painter_drawTile(tile_pointer);
			switch (tile_pointer->draw_state) {
				case FLAGGED:
				{
					_current_flags_count++;
					break;
				}
				case PRESSED:
				{
					tile_pointer->draw_state = SEALED;
					break;
				}
				default:
					break;
			}
		}
	}
	current_flags_count = _current_flags_count;

	Painter_drawBoardMenu(_mines_count, current_flags_count);
}

void GameBoard_onLeftMouseRelease() {
	Tile* tile = findTileUnderCursor();
	if (!tile) {
		return;
	}
	markTileRevealedRecursively(tile);
}

void GameBoard_onLeftMouseDown() {
	Tile* tile = findTileUnderCursor();
	if (!tile) {
		return;
	}
	if (tile->draw_state != REVEALED) {
		return;
	}
	// toggle nearby sealed tiles as pressed
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			Tile* _tile = findTile(tile->x + i, tile->y + j);
			if (_tile->draw_state != SEALED) {
				continue;
			}
			_tile->draw_state = PRESSED;
		}
	}
}

void GameBoard_onRightMouseRelease() {
	Tile* tile = findTileUnderCursor();
	if (tile == NULL) {
		return;
	}
	if (tile->draw_state == FLAGGED) {
		tile->draw_state = SEALED;
		return;
	}
	if (tile->draw_state == REVEALED) {
		// user wants to automate
		uint8_t flagged_count = 0;
		for (int dx = -1; dx <= 1; dx++) {
			for (int dy = -1; dy <= 1; dy++) {
				Tile* _tile = findTile(tile->x + dx, tile->y + dy);
				if (_tile == NULL) {
					continue;
				}
				if (_tile->draw_state == FLAGGED) {
					flagged_count++;
				}
			}
		}
		if (flagged_count == tile->count) {
			// reveal all tiles
			for (int dx = -1; dx <= 1; dx++) {
				for (int dy = -1; dy <= 1; dy++) {
					Tile* _tile = findTile(tile->x + dx, tile->y + dy);
					if (_tile == NULL) {
						continue;
					}
					if (_tile->draw_state != SEALED) {
						continue;
					}
					markTileRevealedRecursively(_tile);
				}
			}
		}
		return;
	}
	if (tile->draw_state != SEALED) {
		return;
	}
	tile->draw_state = FLAGGED;
	checkWinState();
}

static Tile* findTileUnderCursor() {
	const int mouse_x = GetMouseX();
	const int mouse_y = GetMouseY();
	int cell_x = mouse_x / TILE_SIZE;
	int cell_y = mouse_y / TILE_SIZE;
	bool in_boundary = cell_x >= 0 &&
			cell_x < BOARD_WIDTH &&
			cell_y >= 0 &&
			cell_y < BOARD_HEIGHT;
	if (in_boundary == false) {
		return NULL;
	}
	return &tiles[cell_x][cell_y];
}

static Tile* findTile(int tile_x, int tile_y) {
	if (tile_x < 0 || tile_x > BOARD_WIDTH - 1) {
		return NULL;
	}
	if (tile_y < 0 || tile_y > BOARD_HEIGHT - 1) {
		return NULL;
	}
	return &tiles[tile_x][tile_y];
}

static void markTileRevealedRecursively(Tile* tile) {
	if (tile->is_bomb) {
		GameWindow_setGameState(LOSE);
		tile->draw_state = BOMB;
		return;
	}
	tile->draw_state = REVEALED;
	uint8_t mines_count = 0;
	for (int dx = -1; dx <= 1; dx++) {
		for (int dy = -1; dy <= 1; dy++) {
			int pos_x = tile->x + dx;
			int pos_y = tile->y + dy;
			if (pos_x >= BOARD_WIDTH || pos_x < 0) {
				continue;
			}
			if (pos_y >= BOARD_HEIGHT || pos_y < 0) {
				continue;
			}
			Tile* _tile = &tiles[pos_x][pos_y];
			if (_tile->is_bomb) {
				mines_count++;
			}
		}
	}
	if (mines_count == 0) {
		// no mines nearby, repeat the process
		for (int dx = -1; dx <= 1; dx++) {
			for (int dy = -1; dy <= 1; dy++) {
				int pos_x = tile->x + dx;
				int pos_y = tile->y + dy;
				if (pos_x >= BOARD_WIDTH || pos_x < 0) {
					continue;
				}
				if (pos_y >= BOARD_HEIGHT || pos_y < 0) {
					continue;
				}
				Tile* _tile = &tiles[pos_x][pos_y];
				if (_tile->draw_state != SEALED) {
					continue;
				}
				markTileRevealedRecursively(_tile);
			}
		}
	}
	tile->count = mines_count;
}

static void checkWinState() {
	uint8_t mines_n_flagged_count = 0;
	for (int i=0;i<BOARD_WIDTH;i++) {
		for (int j=0;j<BOARD_HEIGHT;j++) {
			Tile* tile = &tiles[i][j];
			if (tile->is_bomb && tile->draw_state == FLAGGED) {
				mines_n_flagged_count++;
			}
		}
	}

	if (mines_n_flagged_count == _mines_count) {
		GameWindow_setGameState(WIN);
	}
}
