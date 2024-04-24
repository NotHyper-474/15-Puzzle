#pragma once
#include "core/math/vectormath.hpp"
#include "core/math/rect.hpp"

struct PhysicsUtils
{
	enum CollisionGroup
	{
		GROUP_STATIC,
		GROUP_DYNAMIC
	};

	static bool AABB(const Rect& a, const Rect& b)
	{
		const SDL_FRect& _a = a.getRectF();
		const SDL_FRect& _b = b.getRectF();
		return AABB(_a.x, _a.y, _a.w, _a.h, _b.x, _b.y, _b.w, _b.h);
	}

	static bool AABB(float x1, float y1, float w1, float h1,
		float x2, float y2, float w2, float h2)
	{
		return x1 + w1 >= x2 &&
			x2 + w2 >= x1 &&
			y1 + h1 >= y2 &&
			y2 + h2 >= y1;
	}

	static bool IsPointInsideAABB(const Vector2f& point, const Rect& rect)
	{
		return (point.x >= rect.x && point.x <= rect.x + rect.width) &&
			(point.y >= rect.y && point.y <= rect.y + rect.height);
	}

private:
	PhysicsUtils() = delete;
	PhysicsUtils& operator=(const PhysicsUtils& other) = delete;
};