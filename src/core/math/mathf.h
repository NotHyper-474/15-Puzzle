#ifndef MATHF_H
#define MATHF_H

#define MATH_PI 3.14159265358979323846264338327950288

#include "vectormath.hpp"
#include <cmath>

namespace Mathf
{
	FORCEINLINE static float Clamp01(float value)
	{
		if (value < 0.f)
			return 0.f;
		else if (value > 1.f)
			return 1.f;
		return value;
	}

	FORCEINLINE static float Lerp(float a, float b, float t)
	{
		return a + (b - a) * Clamp01(t);
	}

	FORCEINLINE static float LerpUnclamped(float a, float b, float t)
	{
		return a + (b - a) * t;
	}

	FORCEINLINE static float InverseLerp(float a, float b, float v)
	{
		return Clamp01((v - a) / (b - a));
	}

	FORCEINLINE static float InverseLerpUnclamped(float a, float b, float v)
	{
		if (a == b) return 0.f;
		return (v - a) / (b - a);
	}

	FORCEINLINE static int RoundToInt(float f) noexcept
	{
		return std::round(f);
	}

	FORCEINLINE static int CeilToInt(float f) noexcept
	{
		return std::ceil(f);
	}

	FORCEINLINE static int FloorToInt(float f) noexcept
	{
		return std::floor(f);
	}
};

#endif