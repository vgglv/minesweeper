#pragma once
#include <stdint.h>
#include "Tile.h"

void Painter_drawTile(const Tile* tile);
void Painter_drawBoardMenu(uint8_t* mines_count, uint8_t* flags_count);
void Painter_drawStageText(uint8_t* stage);

//	class Painter {
//	public:
//		Painter() = delete;
//		~Painter() = delete;
//
//		static void drawTile(Tile* tile);
//		static void drawBoardMenu(uint8_t mines_count, uint8_t flags_count);
//		static void drawStageText(uint8_t stage);
//	private:
//		static void drawNumber(const Tile* tile);
//	};
