#pragma once

#include "c_drawable.h"
#include <SDL_ttf.h>

struct TextShape : C_Drawable
{
	TextShape(const std::string& text, const char* fontPath, int ptsize = 16)
	{
		font = TTF_OpenFont(fontPath, ptsize);
		ptSize = ptsize;
		if (!font)
		{
			Logger::LogError("Could not load font, make sure TTF subsystem is initialized.");
		}
		this->text = text;
		background = { 0, 0, 0, 0 };
	}

	TextShape(const char* text, const char* fontPath, int ptsize = 16) : TextShape((std::string)text, fontPath, ptsize)
	{
	}

	~TextShape()
	{
		TTF_CloseFont(font);
		font = nullptr;
	}

	/// <summary>
	/// Wrapper for TTF_SetFontSize
	/// </summary>
	/// <param name="newPtSize"></param>
	void ChangeSize(int newPtSize)
	{
		if (newPtSize == ptSize) return;
		if (TTF_SetFontSize(font, newPtSize) < 0)
		{
			Logger::LogErrorFormat("Something went wrong while trying to change font size: ", TTF_GetError());
		}
		else ptSize = newPtSize;
	}

	int Draw(SDL_Renderer* renderer) override
	{
		int w = drawRect.width, h = drawRect.height;

		SDL_Surface* ts = TTF_RenderUTF8_Blended_Wrapped(font, text.c_str(), foreground, 200);
		if (ts == nullptr)
		{
			Logger::LogErrorFormat("Something went wrong when rendering text: ", TTF_GetError());
			return -1;
		}

		SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, ts);

		if (w == 0 || h == 0)
		{
			w = ts->w, h = ts->h;
			drawRect.setSize(w + 0.01f, h + 0.01f);
			// TODO: Customizable text centering
			drawRect.position -= Vector2f{ w * .5f, h * .5f};
		}

		SDL_SetRenderDrawColor(renderer, background.r, background.g, background.b, background.a);
		SDL_RenderFillRectF(renderer, drawRect.getRectPtr());

		const int result = SDL_RenderCopyF(renderer, tex, NULL, drawRect.getRectPtr());
		SDL_DestroyTexture(tex);
		SDL_FreeSurface(ts);

		if (autoDestroy) IsActive = false;
		return result;
	}

	TTF_Font* font;
	int ptSize;
	std::string text;
	SDL_Color foreground, background;
	bool autoDestroy = false;
};