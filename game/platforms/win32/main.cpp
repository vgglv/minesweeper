#include <Windows.h>
#include "application/Application.h"

INT WINAPI WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_line, INT cmd_show) {

	auto app = new game::app::Application();
	if (!app->initialize()) {
		return 1;
	}
	app->run();
	app->terminate();

	return 0;
}