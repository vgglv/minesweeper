#include "GameWindow.h"
#include "GameBoard.h"
#include "Constants.h"
#include <limits>
#include <raylib.h>

using namespace game;

namespace {
	static GameState state = GameState::IDLE;
	uint8_t current_mines_diffuculty = 20;
}

void GameWindow::initialize() {
	const int screen_width = BOARD_WIDTH * TILE_SIZE;
	const int screen_height = BOARD_HEIGHT * TILE_SIZE;
	InitWindow(screen_width, screen_height + TILE_SIZE, "Minesweeper");
	BoardSettings settings {
		BOARD_WIDTH,
		BOARD_HEIGHT,
		current_mines_diffuculty
	};
	GameBoard::initialize(settings);
}

void GameWindow::run() {
	while (!WindowShouldClose()) {
		if (state == GameState::QUIT) {
			break;
		}
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			GameBoard::onLeftMouseDown();
		}

		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
			if (!checkIfNeedRestart()) {
				GameBoard::onLeftMouseRelease();
			}
		}

		if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
			if (!checkIfNeedRestart()) {
				GameBoard::onRightMouseRelease();
			}
		}
		BeginDrawing();
		ClearBackground(BLACK);
		GameBoard::draw();
		switch(state) {
			case GameState::WIN:
			{
				const char* win_text = "Stage completed!";
				if (current_mines_diffuculty == std::numeric_limits<uint8_t>::max()) {
					win_text = "You win the game!";
				}
				Vector2 text_size = MeasureTextEx(GetFontDefault(), win_text, TILE_SIZE, 3.F);
				const int screen_width = GameBoard::getBoardWidth() * TILE_SIZE;
				const Vector2 text_pos = { static_cast<float>(screen_width) / 2.F - text_size.x / 2.F, 200.0 };
				Rectangle cell_rect = {text_pos.x - TILE_SIZE, text_pos.y - TILE_SIZE, text_size.x + TILE_SIZE * 2, text_size.y + TILE_SIZE * 2};
				DrawRectangleRec(cell_rect, BLUE);
				DrawRectangleLinesEx(cell_rect, 3.f, BLACK);
				DrawTextEx(GetFontDefault(), win_text, text_pos, TILE_SIZE, 3.F, YELLOW);
				break;
			}
			case GameState::LOSE:
			{
				const char* lose_text = "Game over";
				Vector2 text_size = MeasureTextEx(GetFontDefault(), lose_text, TILE_SIZE, 1.F);
				const int screen_width = GameBoard::getBoardWidth() * TILE_SIZE;
				const Vector2 text_pos = { static_cast<float>(screen_width) / 2.F - text_size.x / 2.F, 200.0 };
				Rectangle cell_rect = {text_pos.x - TILE_SIZE, text_pos.y - TILE_SIZE, text_size.x + TILE_SIZE * 2, text_size.y + TILE_SIZE * 2};
				DrawRectangleRec(cell_rect, BLUE);
				DrawRectangleLinesEx(cell_rect, 3.f, BLACK);
				DrawTextEx(GetFontDefault(), lose_text, text_pos, TILE_SIZE, 1.F, RED);
				break;
			}
			case GameState::IDLE:
			{
				break;
			}
		}
		drawMenu();
		EndDrawing();
	}
}

void GameWindow::terminate() {
	CloseWindow();
}

void GameWindow::setGameState(GameState _state) {
	state = _state;
}

bool GameWindow::checkIfNeedRestart() {
	if (state == GameState::IDLE) {
		return false;
	}
	if (state == GameState::WIN) {
		current_mines_diffuculty++;
		if (current_mines_diffuculty == 0) {
			state = GameState::QUIT;
			return true;
		}
	}
	BoardSettings settings {
		BOARD_WIDTH,
		BOARD_HEIGHT,
		current_mines_diffuculty
	};
	state = GameState::IDLE;
	GameBoard::initialize(settings);
	return true;
}

void GameWindow::drawMenu() {
	GameBoard::drawMenu();

	const char* stage_text = TextFormat("STAGE: %d", current_mines_diffuculty - 19);
	const float bottom_line_y = static_cast<float>(BOARD_HEIGHT * TILE_SIZE) + 2.F;
	const Vector2 stage_text_pos = {TILE_SIZE * 15.F, bottom_line_y};
	DrawTextEx(GetFontDefault(), stage_text, stage_text_pos, TILE_SIZE, 1.5F, RAYWHITE);
}
