#pragma once

#include "vectormath.hpp"
#include <SDL_rect.h>
#include <ostream>
#include <sstream>

struct Rect
{
	constexpr Rect(float x = 0.f, float y = 0.f, float width = 16.f, float height = 16.f) noexcept
	{
		rect = { x, y, width, height };
	}

	constexpr Rect(const Vector2f& position, const Vector2f& size) noexcept : rect { position.x, position.y, size.x, size.y }
	{
	}

	constexpr Rect(const SDL_Rect& rect) noexcept
	{
		this->rect = { float(rect.x), float(rect.y), float(rect.w), float(rect.h) };
	}

	constexpr Rect(const SDL_FRect& rect) noexcept
	{
		this->rect = rect;
	}

#pragma region Properties
	FORCEINLINE const SDL_FRect& getRectF() const
	{
		return rect;
	}

	FORCEINLINE const SDL_FRect* getRectPtr() const
	{
		return &rect;
	}

	FORCEINLINE const Vector2f getPosition() const
	{
		return Vector2f(rect.x, rect.y);
	}

	FORCEINLINE void setPosition(const Vector2f& other)
	{
		rect.x = other.x;
		rect.y = other.y;
	}

	FORCEINLINE void setPosition(float x, float y)
	{
		rect.x = x;
		rect.y = y;
	}

	FORCEINLINE const Vector2f getSize() const
	{
		return Vector2f(rect.w, rect.h);
	}

	FORCEINLINE void setSize(const Vector2f& rhs)
	{
		rect.w = rhs.x;
		rect.h = rhs.y;
	}

	FORCEINLINE void setSize(float w, float h)
	{
		rect.w = w;
		rect.h = h;
	}

	float getX() const
	{
		return rect.x;
	}

	float& setX(float x)
	{
		return rect.x = x;
	}

	float getY() const
	{
		return rect.y;
	}

	float& setY(float y)
	{
		return rect.y = y;
	}

	float getWidth() const
	{
		return rect.w;
	}

	float& setWidth(float width)
	{
		return rect.w = width;
	}

	float getHeight() const
	{
		return rect.h;
	}

	float& setHeight(float height)
	{
		return rect.h = height;
	}

	const Vector2f getCenter() const
	{
		return Vector2f(rect.x + rect.w * .5f, rect.y + rect.h * .5f);
	}

	void setCenter(const Vector2f& rhs)
	{
		rect.x = rhs.x - rect.w * .5f;
		rect.y = rhs.y - rect.h * .5f;
	}

	constexpr const Vector2f getMin() const
	{
		return Vector2f(x, y);
	}

	void setMin(const Vector2f& rhs)
	{
		xMin = rhs.x;
		yMin = rhs.y;
	}

	void setMax(const Vector2f& rhs)
	{
		xMax = rhs.x;
		yMax = rhs.y;
	}

	constexpr float getXMin() const noexcept
	{
		return rect.x;
	}

	constexpr void setXMin(float value)
	{
		const float oldXMax = xMax;
		rect.x = value;
		rect.w = oldXMax - rect.x;
	}

	constexpr float getYMin() const noexcept
	{
		return rect.y;
	}
	
	constexpr void setYMin(float value)
	{
		const float oldYMax = yMax;
		rect.y = value;
		rect.h = oldYMax - rect.y;
	}

	constexpr float getXMax() const noexcept
	{
		return rect.w + rect.x;
	}

	constexpr void setXMax(float value) noexcept
	{
		rect.w = value - rect.x;
	}

	constexpr float getYMax() const noexcept
	{
		return rect.h + rect.y;
	}

	constexpr void setYMax(float value) noexcept
	{
		rect.h = value - rect.y;
	}
#pragma endregion

#pragma region Operators
	bool operator==(const Rect& other) const noexcept
	{
		return SDL_FRectEquals(&rect, &other.rect);
	}
	
	bool operator==(const SDL_Rect& other) const noexcept
	{
		SDL_FRect tmp = { float(other.x), float(other.y), float(other.w), float(other.h) };
		return SDL_FRectEquals(&rect, &tmp);
	}

	bool operator==(const SDL_FRect& other) const noexcept
	{
		return SDL_FRectEquals(&rect, &other);
	}

	bool operator!=(const Rect& other) noexcept
	{
		return !(*this == other);
	}

	Rect& operator+=(const Vector2f& offset)
	{
		rect.x += offset.x;
		rect.y += offset.y;
		return *this;
	}

	Rect& operator-=(const Vector2f& offset)
	{
		rect.x -= offset.x;
		rect.y -= offset.y;
		return *this;
	}

	friend std::ostringstream& operator<<(std::ostringstream& os, const Rect& rect)
	{
		os << "(" << rect.x << ", " << rect.y << ", " << rect.width << ", " << rect.height << ")";
		return os;
	}

	operator SDL_Rect() const
	{
		return SDL_Rect{ static_cast<int>(rect.x), static_cast<int>(rect.y), static_cast<int>(rect.w), static_cast<int>(rect.h) };
	}

	operator SDL_FRect() const
	{
		return rect;
	}

	explicit operator const SDL_FRect* () const
	{
		return &rect;
	}

#pragma endregion

	/// <summary>
	/// Similar to <b>SDL_PointInRect</b>
	/// </summary>
	static bool Contains(const Vector2f& point, const Rect& r)
	{
		return point.x >= r.rect.x &&
			point.x <= r.getXMax() &&
			point.y >= r.rect.y &&
			point.y <= r.getYMax();
	}

	std::string ToString() const
	{
		std::ostringstream oss;
		oss << *this;
		return oss.str();
	}

	__property(getPosition, setPosition) Vector2f position;
	__property(getSize, setSize) Vector2f size;
	__property(getCenter, setCenter) Vector2f center;
	__property(getX, setX) float x;
	__property(getY, setY) float y;
	__property(getWidth, setWidth) float width;
	__property(getHeight, setHeight) float height;

	__property(getXMin, setXMin) float xMin;
	__property(getYMin, setYMin) float yMin;
	__property(getXMax, setXMax) float xMax;
	__property(getYMax, setYMax) float yMax;

private:
	SDL_FRect rect;
};