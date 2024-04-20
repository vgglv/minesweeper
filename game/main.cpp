#include "GameWindow.h"

int main() {
	game::GameWindow::initialize();
	game::GameWindow::run();	
	game::GameWindow::terminate();
	return 0;
}

#ifdef WIN32
#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
	return main();
}
#endif
