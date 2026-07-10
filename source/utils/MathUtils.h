#pragma once
#include <SFML/Graphics.hpp>



namespace MathUtils
{
	inline sf::Vector2f round(const sf::Vector2f& value) { return { std::round(value.x), std::round(value.y) }; }
}