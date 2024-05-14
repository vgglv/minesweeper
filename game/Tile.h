#pragma once
#include <stdbool.h>

typedef enum TileState {
	PRESSED,
	REVEALED,
	SEALED,
	BOMB,
	FLAGGED,
} TileState;

typedef struct Tile {
	int x;
	int y;
	TileState draw_state;
	bool is_bomb;
	bool is_flagged;
	int count;
} Tile;
