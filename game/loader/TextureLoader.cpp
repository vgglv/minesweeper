#include "TextureLoader.h"
#include <iostream>

using namespace game::loader;

namespace {
	SDL_Renderer* renderer = nullptr;
}

SDL_Texture* TextureLoader::loadTexture(const std::string& path) {
	if (!renderer) {
		std::cout << "[TextureLoader] error loading texture " << path <<
			" because rendererer was nullptr!\n";
		return nullptr;
	}
	SDL_Surface* loaded_surface = IMG_Load(path.c_str());
	if (!loaded_surface) {
		std::printf("Unable to load image %s. SDL_Image error: %s",
			path.c_str(), IMG_GetError());
		return nullptr;
	}
	SDL_Texture* new_texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
	if (!new_texture) {
		std::cout << "Unable to create texture: " << path << ". SDL_Error: " <<
			SDL_GetError() << "\n";
		SDL_FreeSurface(loaded_surface);
		return nullptr;
	}
	SDL_FreeSurface(loaded_surface);
	return new_texture;
}

void TextureLoader::setRenderer(SDL_Renderer* _renderer) {
	renderer = _renderer;
}