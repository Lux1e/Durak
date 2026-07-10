#pragma once
#include <SFML/Graphics.hpp>
#include "InputState.h"



class InputController
{
public:
	void update(sf::RenderWindow& window)
	{
		m_inputState.mousePixelPosition = sf::Mouse::getPosition(window);
		sf::Vector2f currentMouseWorldPosition = window.mapPixelToCoords(m_inputState.mousePixelPosition);

		m_inputState.mouseDeltaWorld = currentMouseWorldPosition - m_inputState.mouseWorldPosition;
		m_inputState.mouseWorldPosition = currentMouseWorldPosition;
	}

	const InputState& getInputState() { return m_inputState; }

private:
	InputState m_inputState;
};