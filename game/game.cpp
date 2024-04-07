#include "game.h"
#include <raylib.h>
#include <map>
#include <string>
#include <vector>

using namespace game;

enum class eTexture {
	CLOCK,
	FLAG,
	MINE_HIT,
	MINE,
	NUM_0,
	NUM_1,
	NUM_2,
	NUM_3,
	NUM_4,
	NUM_5,
	NUM_6,
	NUM_7,
	NUM_8,
	TILE
};

namespace {
	std::map<eTexture, Texture2D> textures_map;
	
	std::string getStringPath(eTexture texture_enum) {
		switch (texture_enum) {
			case eTexture::CLOCK:
				return "assets/clock.png";
			case eTexture::FLAG:
				return "assets/flag.png";
			case eTexture::MINE_HIT:
				return "assets/mine_hit.png";
			case eTexture::MINE:
				return "assets/mine.png";
			case eTexture::NUM_0:
				return "assets/n0.png";
			case eTexture::NUM_1:
				return "assets/n1.png";
			case eTexture::NUM_2:
				return "assets/n2.png";
			case eTexture::NUM_3:
				return "assets/n3.png";
			case eTexture::NUM_4:
				return "assets/n4.png";
			case eTexture::NUM_5:
				return "assets/n5.png";
			case eTexture::NUM_6:
				return "assets/n6.png";
			case eTexture::NUM_7:
				return "assets/n7.png";
			case eTexture::NUM_8:
				return "assets/n8.png";
			case eTexture::TILE:
				return "assets/tile.png";
		}
		return {};
	};

	void loadTexture(eTexture texture_enum) {
		const auto& it = textures_map.find(texture_enum);
		if (it != textures_map.end()) {
			return;
		}
		std::string path = getStringPath(texture_enum);
		Image image = LoadImage(path.c_str());
		Texture2D texture = LoadTextureFromImage(image);
		textures_map[texture_enum] = texture;
		UnloadImage(image);
	}
}

void Game::initialize() {
	const int screen_width = 400;
	const int screen_height = 800;
	InitWindow(screen_width, screen_height, "Minesweeper");

	std::vector<eTexture> vector_enums = {
		eTexture::CLOCK,
		eTexture::FLAG,
		eTexture::MINE_HIT,
		eTexture::MINE,
		eTexture::NUM_0,
		eTexture::NUM_1,
		eTexture::NUM_2,
		eTexture::NUM_3,
		eTexture::NUM_4,
		eTexture::NUM_5,
		eTexture::NUM_6,
		eTexture::NUM_7,
		eTexture::NUM_8,
		eTexture::TILE
	};
	for (const auto& tex : vector_enums) {
		loadTexture(tex);
	}
}

void Game::run() {
	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(RAYWHITE);
		int x = 0;
		int y = 0;
		for (const auto& [tex_enum, tex_id] : textures_map) {
			DrawTexture(tex_id, x, y, WHITE);
			x += 32;
			y += 32;
		}
		EndDrawing();
	}
}

void Game::terminate() {
	for (const auto& [texture_enum, texture_id] : textures_map) {
		UnloadTexture(texture_id);
	}
	CloseWindow();
}