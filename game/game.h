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
		int x;
		int y;
		TileState type = TileState::SEALED;
		bool is_bomb = false;
		bool is_flagged = false;
		uint8_t count = 0;
	};

	class Game {
	public:
		Game() = delete;
		~Game() = delete;

		static void initialize();
		static void run();
		static void terminate();

		// may return nullptr
		static Tile* findTileUnderCursor(unsigned int mouse_x, unsigned int mouse_y);
		static Tile* findTile(int tile_x, int tile_y);
	private:
		static void placeMines();
		static void drawTile(const Tile* tile);
		static void onTileLeftButtonRelease(Tile* tile);
		static void onTileLeftButtonDown(Tile* tile);
		static void onTileRightButtonRelease(Tile* tile);
		static void markTileRevealedRecursively(Tile* tile);
		static void checkWinState();
	};

}
