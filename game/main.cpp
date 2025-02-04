#include "raylib.h"
#include "GameBoard.h"
#include <limits>
#include "draw_functions.h"
#include "types.h"

uint8_t current_mines_diffuculty = 20;

bool checkIfNeedRestart() {
	if (current_state == GameState::IDLE) {
		return false;
	}
	if (current_state == GameState::WIN) {
		current_mines_diffuculty++;
		if (current_mines_diffuculty == 0) {
			current_state = GameState::QUIT;
			return true;
		}
	}
	game::BoardSettings settings {
		BOARD_WIDTH,
		BOARD_HEIGHT,
		current_mines_diffuculty
	};
	current_state = GameState::IDLE;
	game::GameBoard::initialize(settings);
	return true;
}


int main() {
	const int screen_width = 800;
	const int screen_height = 640;
	InitWindow(screen_width, screen_height + TILE_SIZE, "Minesweeper");
	game::BoardSettings settings {
		BOARD_WIDTH,
		BOARD_HEIGHT,
		current_mines_diffuculty
	};
	game::GameBoard::initialize(settings);
	while (!WindowShouldClose()) {
		if (current_state == GameState::QUIT) {
			break;
		}
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			game::GameBoard::onLeftMouseDown();
		}

		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
			if (!checkIfNeedRestart()) {
				game::GameBoard::onLeftMouseRelease();
			}
		}

		if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
			if (!checkIfNeedRestart()) {
				game::GameBoard::onRightMouseRelease();
			}
		}
		BeginDrawing();
		ClearBackground(BLACK);
		game::GameBoard::draw();
		switch(current_state) {
		case GameState::WIN: {
			const char* win_text = "Stage completed!";
			if (current_mines_diffuculty == std::numeric_limits<uint8_t>::max()) {
				win_text = "You win the game!";
			}
			Vector2 text_size = MeasureTextEx(GetFontDefault(), win_text, TILE_SIZE, 3.F);
			const int screen_width = game::GameBoard::getBoardWidth() * TILE_SIZE;
			const Vector2 text_pos = { static_cast<float>(screen_width) / 2.F - text_size.x / 2.F, 200.0 };
			Rectangle cell_rect = {text_pos.x - TILE_SIZE, text_pos.y - TILE_SIZE, text_size.x + TILE_SIZE * 2, text_size.y + TILE_SIZE * 2};
			DrawRectangleRec(cell_rect, BLUE);
			DrawRectangleLinesEx(cell_rect, 3.f, BLACK);
			DrawTextEx(GetFontDefault(), win_text, text_pos, TILE_SIZE, 3.F, YELLOW);
			break;
		}
		case GameState::LOSE: {
			const char* lose_text = "Game over";
			Vector2 text_size = MeasureTextEx(GetFontDefault(), lose_text, TILE_SIZE, 1.F);
			const int screen_width = game::GameBoard::getBoardWidth() * TILE_SIZE;
			const Vector2 text_pos = { (static_cast<float>(screen_width) / 2.F) - (text_size.x / 2.F), 200.0 };
			Rectangle cell_rect = {text_pos.x - TILE_SIZE, text_pos.y - TILE_SIZE, text_size.x + (TILE_SIZE * 2), text_size.y + (TILE_SIZE * 2)};
			DrawRectangleRec(cell_rect, BLUE);
			DrawRectangleLinesEx(cell_rect, 3.f, BLACK);
			DrawTextEx(GetFontDefault(), lose_text, text_pos, TILE_SIZE, 1.F, RED);
			break;
		}
		case GameState::IDLE: {
			break;
		}
		default:
			break;
		}
		draw_functions::drawStageText(current_mines_diffuculty - 19);
		EndDrawing();
	}
	CloseWindow();
	return 0;
}

#ifdef WIN32
#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
	return main();
}
#endif
