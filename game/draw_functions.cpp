#include "draw_functions.h"
#include "types.h"

// dependency
#include "raylib.h"

namespace draw_functions {
	void drawTile(const Tile *tile) {
		Rectangle cell_rect = {static_cast<float>(tile->x * TILE_SIZE), static_cast<float>(tile->y * TILE_SIZE), TILE_SIZE, TILE_SIZE};
		switch (tile->state) {
			case TileState::SEALED: {
				DrawRectangleLinesEx(cell_rect, 1.f, WHITE);
				break;
			}
			case TileState::BOMB: {
				DrawRectangleRec(cell_rect, RED);
				DrawRectangleLinesEx(cell_rect, 1.f, WHITE);
				break;
			}
			case TileState::REVEALED: {
				const int x_offset = 10;
				DrawRectangleRec(cell_rect, GRAY);
				if (tile->count > 0) {
					drawNumber(tile);
				}
				DrawRectangleLinesEx(cell_rect, 1.f, WHITE);
				break;
			}
			case TileState::PRESSED: {
				DrawRectangleRec(cell_rect, DARKGRAY);
				DrawRectangleLinesEx(cell_rect, 1.f, WHITE);
				break;
			}
			case TileState::FLAGGED: {
				DrawRectangleRec(cell_rect, BLUE);
				DrawRectangleLinesEx(cell_rect, 1.f, WHITE);
				break;
			}
		}
	}

	void drawNumber(const Tile* tile) {
		const float x = static_cast<float>(tile->x) * static_cast<float>(TILE_SIZE);
		const float y = static_cast<float>(tile->y) * static_cast<float>(TILE_SIZE);
		const float tile_half = static_cast<float>(TILE_SIZE) / 2.F;
		const char* text = TextFormat("%d", tile->count);
		Vector2 text_size = MeasureTextEx(GetFontDefault(), text, TILE_SIZE, 1.F);
		const Vector2 text_pos = {x + tile_half - text_size.x / 2.F, y + tile_half - text_size.y / 2.F + 3.F};
		DrawTextEx(GetFontDefault(), text, text_pos, TILE_SIZE, 1.F, RAYWHITE);
	}

	void drawBoardMenu(uint8_t mines_count, uint8_t flags_count) {
		const char* mines_text = TextFormat("MINES: %d", mines_count);
		const float bottom_line_y = static_cast<float>(BOARD_HEIGHT * TILE_SIZE) + 2.F;
		const Vector2 mines_text_pos = {TILE_SIZE, bottom_line_y};
		DrawTextEx(GetFontDefault(), mines_text, mines_text_pos, TILE_SIZE, 1.5F, RAYWHITE);

		const char* flags_text = TextFormat("FLAGS: %d", flags_count);
		const Vector2 flags_text_pos = {TILE_SIZE * 8.F, bottom_line_y};
		DrawTextEx(GetFontDefault(), flags_text, flags_text_pos, TILE_SIZE, 1.5F, RAYWHITE);
	}

	void drawStageText(uint8_t stage) {
		const char* stage_text = TextFormat("STAGE: %d", stage);
		const float bottom_line_y = static_cast<float>(BOARD_HEIGHT * TILE_SIZE) + 2.F;
		const Vector2 stage_text_pos = {TILE_SIZE * 15.F, bottom_line_y};
		DrawTextEx(GetFontDefault(), stage_text, stage_text_pos, TILE_SIZE, 1.5F, RAYWHITE);
	}
}

