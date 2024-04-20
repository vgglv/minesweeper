#pragma once

namespace game {
	enum class GameState {
		IDLE,
		WIN,
		LOSE,
		QUIT
	};

	class GameWindow {
	public:
		GameWindow() = delete;
		GameWindow(const GameWindow &) = delete;
		GameWindow(GameWindow &&) = delete;
		GameWindow &operator=(const GameWindow &) = delete;
		GameWindow &operator=(GameWindow &&) = delete;
		~GameWindow() = delete;

		static void initialize();
		static void run();
		static void terminate();

		static void setGameState(GameState _state);
	private:
		static bool checkIfNeedRestart();
	};

}
