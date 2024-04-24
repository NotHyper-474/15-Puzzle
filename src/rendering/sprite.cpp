#include "sprite.h"

#include "../core/common.h"
#include "../core/textureManager.h"
#include "../core/logger.hpp"

#include <SDL.h>
#include <cassert>
#include <math.h>

Sprite::Sprite(SDL_Texture* spriteTexture)
{
	assert(spriteTexture != nullptr);
	texture = spriteTexture;
	SDL_QueryTexture(texture, NULL, NULL, &width, &height);
	frameCount = 1;
	drawLayer = DrawLayer::SPRITES;
	pivot = Vector2f(.5f, .5f);
	IsActive = true;
}

Sprite::Sprite(const char* sheetFile, const unsigned int widthPerSprite) :
	Sprite(TextureManager::Instance().LoadFromFile(sheetFile), widthPerSprite)
{
}


Sprite::Sprite(SDL_Texture* spriteSheet, const unsigned int widthPerSprite) :
	Sprite(spriteSheet)
{
	if (widthPerSprite == 0)
		return;
	assert(widthPerSprite >= 0);
	frameCount = ceil(width / widthPerSprite);
}

int Sprite::Draw(SDL_Renderer* renderer) {
	return SDL_RenderCopyF(renderer, texture,
		&drawSrcRect,
		&drawRect.getRectF());
}