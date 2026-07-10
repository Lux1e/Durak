#pragma once
#include <SFML/Graphics.hpp>



namespace CursorsImages
{	
	extern std::optional<sf::Cursor> GradientCursor;
	extern sf::Image GradientImage;
	extern const sf::Vector2u GradientHotSpot;

	void loadImages();
}