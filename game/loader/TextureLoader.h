#pragma once
#include <SDL_image.h>
#include <string>

namespace game::loader {
	class TextureLoader {
	public:
		TextureLoader() = delete;
		~TextureLoader() = delete;

		static SDL_Texture* loadTexture(const std::string& path);
		static void setRenderer(SDL_Renderer* _renderer);
	};
}