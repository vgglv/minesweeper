#include <iostream>
#include "application/Application.h"

int main() {
	auto app = new game::app::Application();
	if (!app->initialize()) {
		return 1;
	}
	app->run();
	app->terminate();
	delete app;
	return 0;
}