#pragma once
#include <SFML/Graphics.hpp>
#include "../core/UIInteractive.h"



class Button : public UIInteractive
{
public:
	Button(sf::Vector2f size = { 160.0f, 90.0f }, sf::Vector2f position = { 0.0f, 0.0f }) : UIInteractive(size, position)
	{
		setChildrenHitTest(false);
	}


	std::optional<UIElement*> getBackgroundElement()
	{
		if (m_getBackgroundElement)
			return m_getBackgroundElement();

		return std::nullopt;
	}

	std::optional<UIElement*> getTextElement()
	{
		if (m_getTextElement)
			return m_getTextElement();

		return std::nullopt;
	}

private:
	friend class ButtonBuilder;

	std::function<UIElement* ()> m_getBackgroundElement;
	std::function<UIElement* ()> m_getTextElement;
};