#pragma once
#include <SFML/Graphics.hpp>



namespace ui::string
{
	void fitTextToRect(sf::Text& string, const sf::Vector2f textRatio, const sf::FloatRect& rect);
}