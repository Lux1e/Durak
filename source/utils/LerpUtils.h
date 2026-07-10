#pragma once
#include <SFML/Graphics.hpp>
#include "../core/Concepts.h"



namespace LerpUtils
{
	template <Lerpable T>
	inline T lerp(const T& a, const T& b, float progress)
	{
		return a + (b - a) * progress;
	}
}