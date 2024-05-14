#pragma once
#include <stdbool.h>

typedef enum {
	PRESSED,
	REVEALED,
	SEALED,
	BOMB,
	FLAGGED,
} TileState;

typedef struct {
	int x;
	int y;
	TileState state;
	bool is_bomb;
	bool is_flagged;
	int count;
} Tile;
