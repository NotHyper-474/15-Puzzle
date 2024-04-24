#ifndef TEXMANAGER_H
#define TEXMANAGER_H

#include "common.h"
#include "../core/hardcoded_data.h"

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>
#include <unordered_map>

struct TextureManager final
{
	using Texture = SDL_Texture*;

	SDL_Renderer* currentRender;
	std::unordered_map<std::string, SDL_Texture*> textureMap;

	static TextureManager& Instance()
	{
		static TextureManager instance;
		return instance;
	}

	SDL_Texture* LoadFromFile(const char* file)
	{
		return LoadFromFile(file, nullptr, nullptr);
	}

	SDL_Texture* LoadFromFile(const char* file, OUT int* width, OUT int* height)
	{
		SDL_assert(currentRender != nullptr);

		SDL_Texture* tex = textureMap[file];
		if (!tex) {
			tex = IMG_LoadTexture(currentRender, file);
			if (!tex)
			{
				GetErrorTexture(file, &tex, NULL);
			}
			else {
				textureMap[file] = tex;
			}
		}
		SDL_QueryTexture(tex, NULL, NULL, width, height);

		return tex;
	}

	void GetErrorTexture(const char* filePath, OUT SDL_Texture** texture, OUT SDL_Surface** surface)
	{
		if (!textureMap["err"] || surface)
		{
			SDL_RWops* io = SDL_RWFromConstMem(Rendering::errIMG, sizeof(Rendering::errIMG) / sizeof(Rendering::errIMG[0]));
			auto* surf = SDL_LoadBMP_RW(io, 1);
			if (!textureMap["err"])
				textureMap["err"] = SDL_CreateTextureFromSurface(currentRender, surf);

			if (surface)
				*surface = surf;
			else // In case textureMap didn't cache it yet
				SDL_FreeSurface(surf);
		}
		std::printf("\033[31mIMG_Load ERROR: \"%s\"\r\nSprite didn't load successfully.\n", IMG_GetError());
		std::cerr << "Failed to load image at path \"" << filePath << "\". Falling back to error texture\033[0m" << std::endl;
		if (texture)
			*texture = textureMap["err"];
	}

	void DestroyTexture(SDL_Texture* texture)
	{
		if (texture == nullptr)
			return;
		for (auto pair = textureMap.begin(); pair != textureMap.end(); pair++)
		{
			if (pair->second == texture)
			{
				SDL_DestroyTexture(pair->second);
				textureMap.erase(pair->first);
				break;
			}
		}
	}

private:
	TextureManager() = default;

	void operator=(const TextureManager&) = delete;
	bool operator==(const TextureManager&) = delete;
};

#endif