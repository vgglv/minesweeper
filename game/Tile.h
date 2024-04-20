#pragma once

namespace game {
	enum class TileState {
		PRESSED,
		REVEALED,
		SEALED,
		BOMB,
		FLAGGED,
	};

	struct Tile {
		int x{};
		int y{};
		TileState state = TileState::SEALED;
		bool is_bomb = false;
		bool is_flagged = false;
		int count = 0;
	};
}
