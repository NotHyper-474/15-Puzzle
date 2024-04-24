#pragma once

#include "sprite.h"


struct SDL_Surface;
struct SDL_Renderer;
// Sprite that can be used with palletes (basically a indexed Sprite)
// These sprites need to hold a copy for both a surface and their textures so they take more memory
// If you don't need palettes use a Sprite instead
// Note: Make sure the first pixel in the sprite sheet is the "transparent" color
struct DynamicSprite : public Sprite
{
	DynamicSprite(const char* spriteSheet, const unsigned int widthPerSprite = 0);
	~DynamicSprite();

	SDL_Palette* GetPalette() const;
	// Wrapper for SDL_SetSurfacePalette
	void SetPalette(SDL_Palette* palette);
	// Wrapper for SDL_SetPaletteColors
	void SetPaletteColors(const SDL_Color* colors, int firstColor, int nColors);

	int Draw(SDL_Renderer* renderer) override;

private:
	FORCEINLINE void HandlePaletteOperation(bool);

	SDL_Surface* surface;
};
