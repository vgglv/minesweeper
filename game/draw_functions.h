#pragma once
#include <cstdint>

struct Tile;

namespace draw_functions {

	void drawTile(const Tile* tile);
	void drawBoardMenu(uint8_t mines_count, uint8_t flags_count);
	void drawStageText(uint8_t stage);
	void drawNumber(const Tile* tile);
}
