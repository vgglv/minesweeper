#pragma once
#include "config.h"

namespace game {
	class Game {
	public:
		explicit Game(GameConfig config);
		~Game() = default;

		void run();
	private:
		void update();
		void draw();

		GameConfig my_config;
	};
}