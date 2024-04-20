#pragma once
#include <cstdint>

namespace game {
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
		TileState type = TileState::SEALED;
		bool is_bomb = false;
		bool is_flagged = false;
		int count = 0;
	};

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
		static void drawMenu();
	private:
		// may return nullptr
		static Tile* findTileUnderCursor();
		static Tile* findTile(int tile_x, int tile_y);
		static void generateBoardTiles();
		static void placeOneMine();
		static void drawTile(const Tile* tile);
		static void markTileRevealedRecursively(Tile* tile);
		static void checkWinState();
		static void drawNumber(const Tile* tile);

		static void autoPlay();
	};
}
