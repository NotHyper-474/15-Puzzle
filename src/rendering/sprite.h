#pragma once

#include "c_drawable.h"
#include "../core/common.h"

struct SDL_Texture;

struct Sprite : public C_Drawable
{
	Sprite(SDL_Texture* spriteTexture);
	Sprite(const char* sheetFile, const unsigned int widthPerSprite = 0);
	Sprite(SDL_Texture* spriteSheet, const unsigned int widthPerSprite);

	FORCEINLINE int getFrameCount() const
	{
		return frameCount;
	}

	FORCEINLINE int getWidth() const { return width; }
	FORCEINLINE int getHeight() const { return height; }

	FORCEINLINE Vector2f getPivot() const { return pivot; }
	FORCEINLINE void setPivot(const Vector2f& _pivot) { pivot = _pivot; }

	FORCEINLINE int Draw(SDL_Renderer* renderer) override;

	SDL_Texture* texture;
	__property(getFrameCount) int FrameCount;
	__property(getWidth) int Width;
	__property(getHeight) int Height;
	__property(getPivot, setPivot) Vector2f Pivot;

protected:
	int width;
	int height;
	int frameCount;
	Vector2f pivot;
};
