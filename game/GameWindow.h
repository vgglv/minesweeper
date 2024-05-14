#pragma once

typedef enum {
	IDLE,
	WIN,
	LOSE,
	QUIT
} GameState;

void GameWindow_initialize();
void GameWindow_run();
void GameWindow_terminate();
void GameWindow_setGameState(GameState _state);

//	class GameWindow {
//	public:
//		GameWindow() = delete;
//		GameWindow(const GameWindow &) = delete;
//		GameWindow(GameWindow &&) = delete;
//		GameWindow &operator=(const GameWindow &) = delete;
//		GameWindow &operator=(GameWindow &&) = delete;
//		~GameWindow() = delete;
//
//		static void initialize();
//		static void run();
//		static void terminate();
//
//		static void setGameState(GameState _state);
//	private:
//		static bool checkIfNeedRestart();
//	};

//}
