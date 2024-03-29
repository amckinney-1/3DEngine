#pragma once

namespace nEngine
{
	const float TwoPi = 6.28318530718f;     // 360 degrees
	const float Pi = 3.14159265359f;        // 180 degrees
	const float HalfPi = 1.57079632679f;    // 90 degrees
	const float QuarterPi = 0.78539816339f; // 45 degrees

	inline float RadToDeg(float radians)
	{
		return radians * 180.0f / Pi;
	}

	inline float DegToRad(float degrees)
	{
		return degrees * Pi / 180.0f;
	}

	template<typename T>
	T Clamp(T value, T min, T max)
	{
		if (value < min) return min;
		if (value > max) return max;

		return value;
	}
	
	template<typename T>
	T Wrap(T value, T min, T max)
	{
		if (value < min) return max;
		if (value > max) return min;

		return value;
	}

	template<typename T>
	T Lerp(T a, T b, float t) // Linear Interpolation
	{
		t = Clamp(t, 0.0f, 1.0f);

		return a + ((b - a) * t);
	}
}