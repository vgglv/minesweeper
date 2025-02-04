#pragma once
#include <cstdint>

struct Tile;

namespace game {
	struct BoardSettings {
		uint8_t board_width;
		uint8_t board_height;
		uint8_t mines_count;
	};

	class GameBoard {
	public:
		static void initialize(const BoardSettings& _settings);
		static void draw();

		static void onLeftMouseRelease();
		static void onLeftMouseDown();
		static void onRightMouseRelease();

		static uint8_t getBoardWidth();
	private:
		// may return nullptr
		static Tile* findTileUnderCursor();
		static Tile* findTile(int tile_x, int tile_y);
		static void generateBoardTiles();
		static void placeOneMine();
		static void markTileRevealedRecursively(Tile* tile);
		static void checkWinState();

		static void autoPlay();
	};
}
