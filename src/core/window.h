#pragma once

#include "common.h"
#include "globals.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include "../rendering/drawSorter.h"
#include "engine.h"

#include <memory>

struct Window
{
	Window(unsigned short width, unsigned short height, SDL_Window* window, SDL_Renderer* renderer, float scale = 1.f)
	{
		SDL_assert(width != 0 || height != 0);
		this->width = width;
		this->height = height;
		this->window = window;
		this->renderer = renderer;
		this->drawSorter = std::make_unique<DrawSorter>(this);
		this->SetScale(scale);
	}

	~Window()
	{
		drawSorter.reset();
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);

		// Should be done here or crash happens
		// The crash happens because the font closing in DrawSorter occurs after the TTF_Quit call
		// TODO: This should not happen
		TTF_Quit();
	}

	void HandleEvents(const SDL_Event* event);

	void SetSize(unsigned newWidth, unsigned newHeight)
	{
		SDL_assert(newWidth != 0 && newHeight != 0);
		width = newWidth, height = newHeight;
		SDL_SetWindowSize(window, newWidth, newHeight);
	}

	void SetPosition(int x, int y) { SDL_SetWindowPosition(window, x, y); }

	void SetScale(float scale) { this->scale = scale; SDL_RenderSetScale(renderer, scale, scale); }
	float GetScale() const { return scale; }

	FORCEINLINE SDL_Window* GetSDLWindow() const { return window; }
	FORCEINLINE SDL_Renderer* GetRenderer() const { return renderer; }
	FORCEINLINE DrawSorter* GetSorter() { return drawSorter.get(); }
	constexpr FORCEINLINE int GetWidth() const { return width; }
	constexpr FORCEINLINE int GetHeight() const { return height; }
	FORCEINLINE Vector2<uint16_t> GetSize() const { return { width, height }; }
	FORCEINLINE Vector2f GetScaledSize() const { return Vector2f(width * scale, height * scale); }
	FORCEINLINE float GetScaledWidth() const { return width * scale; }
	FORCEINLINE float GetScaledHeight() const { return height * scale; }
	FORCEINLINE float GetAspectRatio() const { return float(width) / height; }

private:
	Uint16 width;
	Uint16 height;
	float scale;
	SDL_Renderer* renderer;
	SDL_Window* window;
	std::unique_ptr<DrawSorter> drawSorter;
};