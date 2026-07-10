#pragma once
#include "../core/UIElement.h"
#include "../../resources/Fonts.h"
#include "../../utils/StringUtils.h"



class TextElement : public UIElement
{
public:
	TextElement(const sf::Font& font = Fonts::Arial, std::string string = "", sf::Vector2f size = { 160.0f, 90.0f }, sf::Vector2f position = { 0.0f, 0.0f }) :
		UIElement(size, position), m_text(font, std::move(string), static_cast<unsigned int>(size.y))
	{
		fitTextToRect();
	}

	sf::Text& text() { return m_text; }

	void setString(std::string text) { m_text.setString(std::move(text)); fitTextToRect(); }
	std::string getString() const { return m_text.getString(); }

	void setFont(const sf::Font& font) { m_text.setFont(font); fitTextToRect(); }
	const sf::Font& getFont() const { return m_text.getFont(); }

	void setOutline(float thickness, sf::Color color)
	{
		m_text.setOutlineColor(color);
		m_text.setOutlineThickness(thickness);
		fitTextToRect();
	}

private:
	sf::Text m_text;

	void fitTextToRect() { ui::string::fitTextToRect(m_text, { 1.0f, 1.0f }, getLocalBounds()); }

	void onSizeChanged(sf::Vector2f ratio) override { fitTextToRect(); }
	void drawSelf(sf::RenderTarget& target, sf::RenderStates states) const override { target.draw(m_text, states); }
};