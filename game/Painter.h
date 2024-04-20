#pragma once
#include <cstdint>

namespace game {
	struct Tile;

	class Painter {
	public:
		Painter() = delete;
		~Painter() = delete;

		static void drawTile(Tile* tile);
		static void drawBoardMenu(uint8_t mines_count, uint8_t flags_count);
		static void drawStageText(uint8_t stage);
	private:
		static void drawNumber(const Tile* tile);
	};
}
