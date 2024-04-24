#include "vectormath.hpp"

#include "SDL.h"
#include <float.h>
#include <cmath>
#include <memory>

#include "mathf.h"


#pragma region Vector2f Methods

#pragma region Methods

void Vector2f::Normalize()
{
	(*this) = this->Normalized();
}

inline Vector2f Vector2f::Normalized() const
{
	float len = sqrtf(x * x + y * y);
	// Length sometimes is 0 which divides into NaN
	if (len < FLT_EPSILON)
		return Vector2f::zero();
	return Vector2f(x / len, y / len);
}

Vector2f::operator SDL_FPoint() const
{
	return SDL_FPoint{ x, y };
}

Vector2f::operator SDL_Point() const
{
	return SDL_Point{ static_cast<int>(x), static_cast<int>(y) };
}

std::string Vector2f::ToString() const
{
	std::ostringstream buffer;
	buffer << "(" << x << ", " << y << ")";
	return buffer.str();
}
#pragma endregion


#pragma region Static_Methods
float Vector2f::Dot(const Vector2f& lhs, const Vector2f& rhs) {
	return lhs.x * rhs.x + lhs.y * rhs.y;
}

float Vector2f::Distance(const Vector2f& a, const Vector2f& b)
{
	return sqrtf((a.x * a.x - b.x * b.x) + (a.y * a.y - b.y * b.y));
}

float Vector2f::SqrDistance(const Vector2f& a, const Vector2f& b)
{
	return (a.x * a.x - b.x * b.x) + (a.y * a.y - b.y * b.y);
}

Vector2f Vector2f::ClampMagnitude(const Vector2f& v, float maxLength)
{
	const float sqrMag = v.x * v.x + v.y * v.y;
	if (sqrMag > maxLength * maxLength)
	{
		float mag = sqrtf(sqrMag);
		float norm_x = v.x / mag;
		float norm_y = v.y / mag;
		return Vector2f(norm_x * maxLength, norm_y * maxLength);
	}
	return v;
}

Vector2f& Vector2f::ClampMagnitude(Vector2f& v, float maxLength) const
{
	const float sqrMag = v.x * v.x + v.y * v.y;
	if (sqrMag > maxLength * maxLength)
	{
		const float mag = sqrtf(sqrMag);
		v /= mag;
		v *= maxLength;
	}
	return v;
}

FORCEINLINE Vector2f Vector2f::Lerp(const Vector2f& a, const Vector2f& b, float t)
{
	t = Mathf::Clamp01(t);
	return Vector2f(
		a.x + (b.x - a.x) * t,
		a.y + (b.y - a.y) * t
	);
}

FORCEINLINE Vector2f Vector2f::LerpUnclamped(const Vector2f& a, const Vector2f& b, float t)
{
	return Vector2f(
		a.x + (b.x - a.x) * t,
		a.y + (b.y - a.y) * t
	);
}

Vector2f Vector2f::Scale(const Vector2f& a, const Vector2f& b)
{
	return Vector2f(a.x * b.x, a.y * b.y);
}

Vector2f Vector2f::Scale(const Vector2f& b) const
{
	return Vector2f(x * b.x, y * b.y);
}
#pragma endregion

#pragma endregion

#pragma region Vector3f Methods
Vector3f::Vector3f(float x, float y, float z) : x(x), y(y), z(z) {}

float Vector3f::Dot(const Vector3f& lhs, const Vector3f& rhs) {
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

float Vector3f::Distance(const Vector3f& lhs, const Vector3f& rhs) {
	return std::sqrtf((lhs.x * lhs.x - rhs.x * rhs.x) + (lhs.y * lhs.y - rhs.y * rhs.y) + (lhs.z * lhs.z - rhs.z * rhs.z));
}
#pragma endregion

