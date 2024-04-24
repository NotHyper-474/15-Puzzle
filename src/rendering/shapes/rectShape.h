#pragma once

#include "../c_drawable.h"
#include "core/math/rect.hpp"
#include "SDL.h"

struct RectShape : public C_Drawable
{
	RectShape(const Rect rect, const SDL_Color color) :
		rect(rect),
		color(color)
	{
	}

	int Draw(SDL_Renderer* renderer) override {
		IsActive = false;
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		return SDL_RenderFillRectF(renderer, (const SDL_FRect*)rect);
	}

private:
	const Rect rect;
	const SDL_Color color;
};