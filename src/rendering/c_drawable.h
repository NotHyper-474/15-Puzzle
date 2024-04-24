#pragma once

#include "render_drawlayer.h"
#include "../core/math/rect.hpp"

struct SDL_Renderer;

struct C_Drawable
{
	enum DrawLayer drawLayer;
	bool IsActive = true;
	int sortOrder = 0;
	SDL_Rect drawSrcRect;
	Rect drawRect;

	virtual ~C_Drawable() = default;

	virtual int Draw(SDL_Renderer* renderer) = 0;
};