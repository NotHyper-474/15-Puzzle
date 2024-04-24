#include "drawSorter.h"

#include <algorithm>

#include "core/window.h"
#include "shapes/rectShape.h"
#include "shapes/textShape.h"
#include "core/logger.hpp"
#include "ECS/components/camera.h"
#include "core/engine.h"
#include <math/mathf.h>


void DrawSorter::Paint()
{
	if (activeCamera == nullptr)
	{
		Logger::LogWarning("No active camera assigned, trying to find one. If no camera is found drawables will not be rendered.");
		activeCamera = Engine::getGame()->FindEntityOfType<Camera>();
		return;
	}

	for (auto& layer : requestMap)
	{
		auto& drawList = layer.second;

		drawList.erase(std::remove_if(drawList.begin(), drawList.end(),
			[](const DrawablePtr& drawable) {
				return !drawable || !drawable->IsActive;
			}), drawList.end());

		std::sort(drawList.begin(), drawList.end(), [](const DrawablePtr& a, const DrawablePtr& b)
			{ return a->sortOrder < b->sortOrder; });

		for (auto& drawable : drawList)
		{
			if (drawable)
			{
				if (layer.first != DrawLayer::UI)
				{
					const Vector2f projection = activeCamera->getProjection();
					drawable->drawRect.x -= projection.x;
					drawable->drawRect.y -= projection.y;
				}
				if (drawable->Draw(window->GetRenderer()) != 0)
				{
					Logger::LogErrorFormat("An internal error might have occurred in SDL: %s", SDL_GetError());
				}
			}
		}

		if (!hasPendingDraws)
			hasPendingDraws = drawList.size() > 0;
	}

	hasPendingDraws = false;
}

void DrawSorter::DrawFillRect(const Rect& rect, const SDL_Color& color)
{
	auto shape = new RectShape(rect, color);
	shape->IsActive = true;
	shape->drawLayer = DrawLayer::DEBUG;
	shape->drawSrcRect = { 0, 0, static_cast<int>(rect.width), static_cast<int>(rect.height) };
	Add(shape);
}

/// <summary>
/// Adds a text to the sorter which should be manually managed and destroyed with IsActive
/// This should be replaced later with some sort of UI Painter
/// </summary>
/// <param name="rect"></param>
/// <param name="text"></param>
/// <param name="font"></param>
/// <param name="ptsize"></param>
/// <param name="fg"></param>
/// <returns></returns>
TextShape* DrawSorter::AddText(const Rect& rect, const std::string& text, const char* font, int ptsize, const SDL_Color& fg)
{
	auto shape = new TextShape(text, font, ptsize);
	shape->IsActive = true;
	shape->drawLayer = DrawLayer::UI;
	shape->foreground = fg;
	shape->drawRect = rect;
	Add(shape);
	return shape;
}