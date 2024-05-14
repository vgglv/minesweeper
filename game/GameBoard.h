#pragma once
#include <stdint.h>

void GameBoard_initialize(const uint8_t mines_count);
void GameBoard_draw();

void GameBoard_onLeftMouseRelease();
void GameBoard_onLeftMouseDown();
void GameBoard_onRightMouseRelease();

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
