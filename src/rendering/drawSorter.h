#pragma once

#include <map>
#include <unordered_map>
#include <vector>
#include <memory>

#include "c_drawable.h"

struct Window;
struct Camera;
struct TextShape;
struct TextData;

/// <summary>
/// This class was originally called "Painter" hence why you can see drawing methods
/// </summary>
struct DrawSorter final
{
	// TYPE ALIASES
	using DrawablePtr = std::unique_ptr<C_Drawable>;
	using DrawableList = std::vector<DrawablePtr>;
	using RequestMap = std::map<DrawLayer, DrawableList>;

	DrawSorter(Window* window) :
		window(window), activeCamera(), requestMap()
	{
	}

#pragma region Drawing Methods
	void DrawFillRect(const Rect& rect, const SDL_Color& color);
	TextShape* AddText(const Rect& rect, const std::string& text, const char* fontPath, int ptsize, const SDL_Color& foreground);

#pragma endregion

	void Paint();

	void Add(DrawablePtr draw)
	{
		if (!draw) return;

		const DrawLayer layer = draw->drawLayer;
		auto itr = requestMap.find(layer);
		if (itr == requestMap.end())
		{
			DrawableList list;
			list.emplace_back(std::move(draw));
			requestMap.insert(std::make_pair(layer, std::move(list)));
			return;
		}

		requestMap[draw->drawLayer].push_back(std::move(draw));
	}

	FORCEINLINE void Add(C_Drawable* draw)
	{
		if (draw)
		{
			Add(DrawablePtr(std::move(draw)));
		}
	}

	bool HasPendingDrawables() const { return hasPendingDraws; }

	private:
	Window* window;
	Camera* activeCamera;
	RequestMap requestMap;
	
	bool hasPendingDraws = false;
};
