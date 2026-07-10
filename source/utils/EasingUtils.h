#pragma once
#include <SFML/Graphics.hpp>



namespace Easing
{
	inline float linear(float progress) { return progress; }
	inline float easeInCubic(float progress) { return progress * progress * progress; }
	inline float easeOutCubic(float progress) { float x = 1 - progress; return 1 - (x * x * x); }
	inline float easeInOutCubic(float progress)
	{
		if (progress < 0.5f)
			return easeInCubic(progress * 2) / 2;
		else
			return easeOutCubic(progress * 2 - 1.0f) / 2 + 0.5f;
	}
}