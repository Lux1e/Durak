#pragma once
#include <concepts>
#include "animation/Animation.h"



template <typename T>
concept Lerpable =
	requires(const T & a, const T & b, float t)
{
	{ a + (b - a) * t } -> std::convertible_to<T>;
};

template<typename T>
concept AnimationType = std::derived_from<T, Animation>;