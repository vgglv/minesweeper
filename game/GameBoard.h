#pragma once
#include <stdint.h>

static void initialize(const uint8_t mines_count);
static void draw();

static void onLeftMouseRelease();
static void onLeftMouseDown();
static void onRightMouseRelease();

static uint8_t getBoardWidth();

//	class GameBoard {
//	public:
//	private:
//		// may return nullptr
//		static Tile* findTileUnderCursor();
//		static Tile* findTile(int tile_x, int tile_y);
//		static void generateBoardTiles();
//		static void placeOneMine();
//		static void markTileRevealedRecursively(Tile* tile);
//		static void checkWinState();
//
//		static void autoPlay();
//	};
//}
