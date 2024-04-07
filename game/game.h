#pragma once

namespace game {
	class Game {
	public:
		Game() = delete;
		~Game() = delete;

		static void initialize();
		static void run();
		static void terminate();
	};

}