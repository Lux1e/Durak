#pragma once
#include <SFML/Graphics.hpp>
#include "../core/UIElement.h"
#include "../../resources/Fonts.h"



//class Label : public UIElement
//{
//public:
//	Label(const std::string& text = "", const sf::Font& font = Fonts::Arial, unsigned int characterSize = 30U) : m_text(font, text, characterSize)
//	{
//		setCenterOrigin();
//	}
//
//
//	void drawSelf(sf::RenderTarget& target, sf::RenderStates states) const override { target.draw(m_text); }
//
//	void setString(const std::string& str) { m_text.setString(str); }
//	sf::Text& getText() { return m_text; }
//
//	void setCenterOrigin()
//	{
//		auto bounds = m_text.getLocalBounds();
//		m_text.setOrigin({ bounds.position.x + m_text.getLocalBounds().size.x / 2, bounds.position.y + m_text.getLocalBounds().size.y / 2 });
//	}
//
//private:
//	sf::Text m_text;
//};