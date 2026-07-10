#pragma once
#include <SFML/Graphics.hpp>



class InputState
{
public:
	sf::Vector2i mousePixelPosition;
	sf::Vector2f mouseWorldPosition;
	sf::Vector2f mouseDeltaWorld;
};