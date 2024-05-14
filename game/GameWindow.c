#include "GameWindow.h"
#include "GameBoard.h"
#include "Constants.h"
#include "Painter.h"
#include "raylib.h"
#include <stdio.h>

const uint8_t INITIAL_AMOUNT_OF_MINE_DIFFICULTY = 20;

static GameState state = IDLE;
static uint8_t current_mines_diffuculty = INITIAL_AMOUNT_OF_MINE_DIFFICULTY;

bool checkIfNeedRestart();

void GameWindow_initialize() {
	const int screen_width = BOARD_WIDTH * TILE_SIZE;
	const int screen_height = BOARD_HEIGHT * TILE_SIZE;
	InitWindow(screen_width, screen_height + TILE_SIZE, "Minesweeper");
	GameBoard_initialize(current_mines_diffuculty);
}

void GameWindow_run() {
	while (!WindowShouldClose()) {
		if (state == QUIT) {
			break;
		}
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			GameBoard_onLeftMouseDown();
		}

		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
			if (!checkIfNeedRestart()) {
				GameBoard_onLeftMouseRelease();
			}
		}

		if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
			if (!checkIfNeedRestart()) {
				GameBoard_onRightMouseRelease();
			}
		}
		BeginDrawing();
		ClearBackground(BLACK);
		GameBoard_draw();
		switch(state) {
			case WIN:
			{
				const char* win_text = "Stage completed!";
				if (current_mines_diffuculty == UINT8_MAX) {
					win_text = "You win the game!";
				}
				Vector2 text_size = MeasureTextEx(GetFontDefault(), win_text, TILE_SIZE, 3.F);
				const int screen_width = BOARD_WIDTH * TILE_SIZE;
				const Vector2 text_pos = { 
					.x = screen_width / 2.F - text_size.x / 2.F, 
					.y = 200.0 
				};
				Rectangle cell_rect = {text_pos.x - TILE_SIZE, text_pos.y - TILE_SIZE, text_size.x + TILE_SIZE * 2, text_size.y + TILE_SIZE * 2};
				DrawRectangleRec(cell_rect, BLUE);
				DrawRectangleLinesEx(cell_rect, 3.f, BLACK);
				DrawTextEx(GetFontDefault(), win_text, text_pos, TILE_SIZE, 3.F, YELLOW);
				break;
			}
			case LOSE:
			{
				const char* lose_text = "Game over";
				Vector2 text_size = MeasureTextEx(GetFontDefault(), lose_text, TILE_SIZE, 1.F);
				const int screen_width = BOARD_WIDTH * TILE_SIZE;
				const Vector2 text_pos = { (float)(screen_width) / 2.F - text_size.x / 2.F, 200.0 };
				Rectangle cell_rect = {text_pos.x - TILE_SIZE, text_pos.y - TILE_SIZE, text_size.x + TILE_SIZE * 2, text_size.y + TILE_SIZE * 2};
				DrawRectangleRec(cell_rect, BLUE);
				DrawRectangleLinesEx(cell_rect, 3.f, BLACK);
				DrawTextEx(GetFontDefault(), lose_text, text_pos, TILE_SIZE, 1.F, RED);
				break;
			}
			case IDLE:
			{
				break;
			}
		}
		Painter_drawStageText(current_mines_diffuculty - INITIAL_AMOUNT_OF_MINE_DIFFICULTY + 1);
		EndDrawing();
	}
}

void GameWindow_terminate() {
	CloseWindow();
}

void GameWindow_setGameState(GameState _state) {
	state = _state;
}

bool checkIfNeedRestart() {
	if (state == IDLE) {
		return false;
	}
	if (state == WIN) {
		current_mines_diffuculty++;
		if (current_mines_diffuculty == 0) {
			state = QUIT;
			return true;
		}
	}
	GameBoard_initialize(current_mines_diffuculty);
	state = IDLE;
	return true;
}
