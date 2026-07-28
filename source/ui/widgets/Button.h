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


	UIElement* getBackgroundElement()
	{
		if (m_getBackgroundElement)
			return m_getBackgroundElement(*this);

		return nullptr;
	}

	UIElement* getTextElement()
	{
		if (m_getTextElement)
			return m_getTextElement(*this);

		return nullptr;
	}

	UIElement* getSpriteElement()
	{
		if (m_getSpriteElement)
			return m_getSpriteElement(*this);

		return nullptr;
	}

private:
	friend class ButtonBuilder;

	std::function<UIElement* (Button&)> m_getBackgroundElement;
	std::function<UIElement* (Button&)> m_getTextElement;
	std::function<UIElement* (Button&)> m_getSpriteElement;


	void drawSelf(sf::RenderTarget& target, sf::RenderStates states) const override {}
};