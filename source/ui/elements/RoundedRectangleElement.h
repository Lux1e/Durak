#pragma once
#include "../core/UIElement.h"
#include "../primitives/RoundedRectangleShape.h"



class RoundedRectangleElement : public UIElement
{
public:
	RoundedRectangleElement(sf::Vector2f size = { 160.0f, 90.0f }, sf::Vector2f position = { 0.0f, 0.0f }) : UIElement(size, position)
	{
		m_shape.setSize(size);
	}

	RoundedRectangleShape& shape() { return m_shape; }

private:
	RoundedRectangleShape m_shape;

	void onSizeChanged(sf::Vector2f ratio) override { m_shape.setSize(getSize()); }
	void drawSelf(sf::RenderTarget& target, sf::RenderStates states) const override { target.draw(m_shape, states); }
};