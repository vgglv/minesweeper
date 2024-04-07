#include <iostream>
#include "game.h"

int main() {
	game::Game::initialize();
	game::Game::run();	
	game::Game::terminate();
	return 0;
}

#ifdef WIN32
#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
	return main();
}
#endif
