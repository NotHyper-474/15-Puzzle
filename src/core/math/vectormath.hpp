#pragma once

#include "core/common.h"

#include <array>
#include <algorithm>
#include <iostream>
#include <sstream>

struct SDL_FPoint;
struct SDL_Point;

struct Vector2f {
	constexpr Vector2f() {}
	constexpr Vector2f(float x, float y) noexcept : x(x), y(y) {}
	constexpr Vector2f(const Vector2f& other) : x(other.x), y(other.y) {}

	float Magnitude() const { return std::sqrtf(x * x + y * y); }
	constexpr float SqrMagnitude() const { return x * x + y * y; }

	void Normalize();
	Vector2f Normalized() const;
	std::string ToString() const;

	void setX(float X) { x = X; }
	void setY(float Y) { y = Y; }
	void Set(float X, float Y) { x = X; y = Y; }

	static float Dot(const Vector2f& lhs, const Vector2f& rhs);
	static float Distance(const Vector2f& lhs, const Vector2f& rhs);
	static float SqrDistance(const Vector2f& lhs, const Vector2f& rhs);
	static Vector2f ClampMagnitude(const Vector2f& v, float maxLength);
	Vector2f& ClampMagnitude(Vector2f& v, float maxLength) const;

	static Vector2f Lerp(const Vector2f& a, const Vector2f& b, float t);
	static Vector2f LerpUnclamped(const Vector2f& a, const Vector2f& b, float t);

	static Vector2f Scale(const Vector2f& a, const Vector2f& b);
	Vector2f Scale(const Vector2f& b) const;

	static constexpr Vector2f zero()
	{
		constexpr Vector2f zeroVector{ 0,0 };
		return zeroVector;
	}
	static constexpr Vector2f one() {

		constexpr Vector2f oneVector(1.f, 1.f);
		return oneVector;
	}


#pragma region Operators

	void operator=(const Vector2f& rhs)
	{
		x = rhs.x;
		y = rhs.y;
	}

	bool operator==(const Vector2f& rhs) const
	{
		constexpr float kEpsilon = 10e-6f;
		const float diff_x = x - rhs.x;
		const float diff_y = y - rhs.y;
		return (diff_x * diff_x + diff_y * diff_y) < kEpsilon * kEpsilon;
	}

	bool operator!=(const Vector2f& rhs) const
	{
		return !(*this == rhs);
	}

	Vector2f operator-() const
	{
		Vector2f v;
		v.Set(-x, -y);
		return v;
	}

	constexpr Vector2f& operator+=(const Vector2f& rhs) noexcept
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	constexpr Vector2f& operator-=(const Vector2f& rhs) noexcept
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	constexpr Vector2f& operator*=(float rhs) noexcept
	{
		x *= rhs;
		y *= rhs;
		return *this;
	}

	constexpr Vector2f& operator/=(float rhs) noexcept
	{
		x /= rhs;
		y /= rhs;
		return *this;
	}

	float& operator[](const size_t i)
	{
		if (i > 1)
		{
			Logger::LogErrorFormat("Index out of range: %zu. Limit is 1", i);
			throw std::out_of_range("Index out of range.");
		}

		return *(&x + i);
	}

	float operator[](const size_t i) const
	{
		if (i > 1)
		{
			Logger::LogErrorFormat("Index out of range: %zu. Limit is 1", i);
			throw;
		}

		return *(&x + i);
	}

	operator SDL_FPoint() const;
	operator SDL_Point() const;

#pragma endregion

	float x, y;
};

constexpr Vector2f operator+(const Vector2f& lhs, const Vector2f& rhs)
{
	return Vector2f(lhs.x + rhs.x, lhs.y + rhs.y);
}

constexpr Vector2f operator-(const Vector2f& lhs, const Vector2f& rhs)
{
	return Vector2f(lhs.x - rhs.x, lhs.y - rhs.y);
}

constexpr Vector2f operator*(const Vector2f& lhs, float rhs)
{
	return Vector2f(lhs.x * rhs, lhs.y * rhs);
}

constexpr Vector2f operator/(const Vector2f& lhs, float rhs)
{
	return Vector2f(lhs.x / rhs, lhs.y / rhs);
}

struct Vector3f {
	Vector3f(float x = 0.f, float y = 0.f, float z = 0.f);

	void setX(float X) { x = X; }
	void setY(float Y) { y = Y; }
	void setZ(float Z) { z = Z; }

	static float Dot(const Vector3f& lhs, const Vector3f& rhs);
	static float Distance(const Vector3f& lhs, const Vector3f& rhs);

#pragma region Operators
	operator Vector2f() const
	{
		return Vector2f(x, y);
	}
#pragma endregion

	float x, y, z;
};

struct Vector4f
{
	static float Dot(const Vector4f& lhs, const Vector4f& rhs)
	{
		return lhs.x * rhs.y + lhs.x * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
	}

	float operator[](const size_t i) const
	{
		if (i >= 4) {
			std::cerr << "\x1B[31mIndex out of range: " << i << ". Limit is 3\33[0m\t" << std::endl;
			throw;
		}
		return *(&x + i);
	}

	float& operator[](const size_t i)
	{
		if (i >= 4) {
			std::cerr << "\x1B[31mIndex out of range: " << i << ". Limit is 3\33[0m\t" << std::endl;
			throw std::out_of_range("");
		}

		return *(&x + i);
	}

	float x, y, z, w;
};

template<class T, size_t L> requires std::is_arithmetic<T>::value
struct Vector
{
	constexpr Vector(std::same_as<T> auto ... init) : values { init... }
	{
		// Can't use requires (sizeof...(init) == L) for some reason
		// So we have to trust the programmer to be using the same amount of values
	}

	Vector() {}

	Vector(bool initialize)
	{
		if (!initialize) return;
		values.fill(0);
	}

	inline T& operator[](size_t i)
	{
		if (i >= L) {
			throw std::out_of_range("Index out of bounds");
		}
		return values[i];
	}

	inline T operator[](size_t i) const
	{
		if (i >= L) {
			throw std::out_of_range("Index out of bounds");
		}
		return values[i];
	}

#pragma region Methods
	virtual float Distance() const
	{
		T sum = 0;
		for (size_t i = 0; i < L; ++i)
			sum += values[i] * values[i];
		return sqrt(sum);
	}

	virtual T SqrDistance() const
	{
		T result = 0;
		for (size_t i = 0; i < L; ++i)
			result += values[i] * values[i];
		return result;
	}
#pragma endregion

protected:
	std::array<T, L> values;
};


template<typename T>
struct Vector2 : Vector<T, 2>
{
	constexpr Vector2(T x, T y) : Vector<T, 2>(x, y) {

	}

	Vector2() : Vector<T, 2>() {}

	Vector2 operator+(const Vector2& rhs) const {
		return Vector2(getX() + rhs.getX(), getY() + rhs.getY());
	}

	Vector2 operator-(const Vector2& rhs) const {
		return Vector2(getX() - rhs.getX(), getY() - rhs.getY());
	}

	template<typename _Other>
	Vector2 operator-(const Vector2<_Other>& rhs) const {
		return Vector2(getX() - rhs.getX(), getY() - rhs.getY());
	}

	constexpr Vector2 operator-() const {
		return Vector2(-getX(), -getY());
	}

	Vector2f operator*(float scale) const {
		return Vector2f(getX() * scale, getY() * scale);
	}

	constexpr bool operator==(const Vector2& rhs) const {
		return getX() == rhs.getX() && getY() == rhs.getY();
	}

	operator Vector2f() const {
		return Vector2f{ (float)getX(), (float)getY() };
	}

	template<class _Other>
	explicit operator Vector2<_Other>() const {
		return Vector2<_Other>((_Other)getX(), (_Other)getY());
	}

	float Distance() const override
	{
		return std::sqrtf(this->values[0] * this->values[0] + this->values[1] * this->values[1]);
	}

	T SqrDistance() const override
	{
		return this->values[0] * this->values[0] + this->values[1] * this->values[1];
	}

	constexpr static std::size_t size() { return 2; }

	constexpr T getX() const { return this->values[0]; }
	constexpr T getY() const { return this->values[1]; }
	constexpr void setX(const T _x) { this->values[0] = _x; }
	constexpr void setY(const T _y) { this->values[1] = _y; }
	void setValues(const T x, const T y) { this->values[0] = x; this->values[1] = y; }

#ifndef __GNUC__
	__property(getX, setX) T x;
	__property(getY, setY) T y;
#endif
};

typedef Vector2<int> Vector2i;
typedef Vector2<uint32_t> Vector2ui;
typedef Vector2<size_t> Vector2zu;