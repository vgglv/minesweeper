#include <iostream>
#include "game.h"

int main() {
	game::GameConfig game_config;
	game_config.screen_width = 800;
	game_config.screen_height = 800;
	game_config.target_fps = 60;

	game::Game my_game(game_config);
	my_game.run();

	return 0;
}

#ifdef WIN32
#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, int nCmdShow) {
    return main();
}
#endif
