#include "game.h"
#include <raylib.h>

using namespace game;


Game::Game(GameConfig config) : my_config(config) {

}

void Game::update() {

}

void Game::draw() {
	BeginDrawing();
	ClearBackground(RAYWHITE);
	EndDrawing();
}

void Game::run() {
	InitWindow(
		my_config.screen_width,
		my_config.screen_height,
		"Minesweeper"
	);

	SetTargetFPS(my_config.target_fps);

	while(!WindowShouldClose()) {
		update();
		draw();
	}

	CloseWindow();
}