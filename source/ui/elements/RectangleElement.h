#pragma once
#include "../core/UIElement.h"



class RectangleElement : public UIElement
{
public:
	RectangleElement(sf::Vector2f size = { 160.0f, 90.0f }, sf::Vector2f position = { 0.0f, 0.0f }) : UIElement(size, position)
	{
		m_shape.setSize(size);
	}

	void onSizeChanged(sf::Vector2f ratio) override { m_shape.setSize(getSize()); }
	sf::RectangleShape& shape() { return m_shape; }

private:
	sf::RectangleShape m_shape;

	void drawSelf(sf::RenderTarget& target, sf::RenderStates states) const override { target.draw(m_shape, states); }
};