#include "dynamicSprite.h"
#include "SDL.h"
#include "../core/textureManager.h"

#include <vector>
#include <math.h>

constexpr unsigned PaletteSize = 256;


DynamicSprite::DynamicSprite(const char* spriteSheet, const unsigned int widthPerSprite) : Sprite(*this)
{
	SDL_Renderer* renderer = TextureManager::Instance().currentRender;
	surface = IMG_Load(spriteSheet);

	if (!surface)
	{
		// Cached texture shouldn't be obtained or else it'll be destroyed alongside the sprite
		TextureManager::Instance().GetErrorTexture(spriteSheet, NULL, &surface);
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_QueryTexture(texture, NULL, NULL, &width, &height);
		frameCount = 1;
		return;
	}

	SDL_Surface* converted = SDL_CreateRGBSurface(0, surface->w, surface->h, 8, 0, 0, 0, 0);

	if (!converted)
	{
		Logger::LogErrorFormat("Failed to create converted surface: %s", SDL_GetError());
		return;
	}

	SDL_LockSurface(converted);
	SDL_LockSurface(surface);

	std::vector<SDL_Color> paletteColors;
	paletteColors.reserve(PaletteSize);
	for (size_t y = 0; y < static_cast<size_t>(surface->h); y++)
	{
		for (size_t x = 0; x < static_cast<size_t>(surface->w); x++)
		{
			const size_t i = surface->w * y + x;
			Uint32 pixel = ((Uint32*)surface->pixels)[i];

			Uint8 r, g, b, a;
			size_t index = -1;
			SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
			const SDL_Color color = { r, g, b, a };

			for (size_t j = 0; j < paletteColors.size(); j++)
			{
				const SDL_Color& paletteColor = paletteColors[j];
				if (paletteColor.r == r &&
					paletteColor.g == g &&
					paletteColor.b == b &&
					paletteColor.a == a)
				{
					index = j;
					break;
				}
			}
			if (index == static_cast<size_t>(-1))
			{
				paletteColors.push_back(color);
				index = paletteColors.size() - 1;
			}
			((Uint8*)converted->pixels)[i] = (Uint8)index;
		}
	}
	SDL_SetColorKey(converted, SDL_TRUE, 0);
	SDL_SetPaletteColors(converted->format->palette, paletteColors.data(), 0, PaletteSize);

	SDL_UnlockSurface(converted);
	SDL_UnlockSurface(surface);

	SDL_FreeSurface(surface);
	surface = converted;
	texture = SDL_CreateTextureFromSurface(renderer, surface);

	if (!texture)
	{
		Logger::LogWarningFormat("Could not create texture for DynamicSprite: \"%s\"! Proceeding anyway.", SDL_GetError());
	}

	width = surface->w;
	height = surface->h;

	IsActive = true;

	drawLayer = DrawLayer::SPRITES;
	drawSrcRect = { 0, 0, width, height };
	drawRect = { 0.f, 0.f, (float)width, (float)height };
	pivot = Vector2f::one() * .5f;

	if (widthPerSprite <= 0)
	{
		frameCount = 1;
		return;
	}
	frameCount = std::ceil(width / widthPerSprite);
}

DynamicSprite::~DynamicSprite()
{
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
	IsActive = false;
}

void DynamicSprite::HandlePaletteOperation(bool success)
{
	if (!success)
	{
		Logger::LogErrorFormat("Error when setting palette: %s", SDL_GetError());
		return;
	}

	SDL_DestroyTexture(texture);
	texture = nullptr;
}

SDL_Palette* DynamicSprite::GetPalette() const
{
	return surface->format->palette;
}

void DynamicSprite::SetPalette(SDL_Palette* palette)
{
	HandlePaletteOperation(SDL_SetSurfacePalette(surface, palette) == 0);
}

void DynamicSprite::SetPaletteColors(const SDL_Color* colors, int firstColor, int nColors)
{
	if (!surface || !surface->format->palette) return;
	HandlePaletteOperation(SDL_SetPaletteColors(surface->format->palette, colors, firstColor, nColors) == 0);
}

int DynamicSprite::Draw(SDL_Renderer* renderer)
{
	if (!surface) return 0;
	if (!texture)
		texture = SDL_CreateTextureFromSurface(renderer, surface);
	return Sprite::Draw(renderer);
}