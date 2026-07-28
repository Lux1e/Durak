#pragma once
#include "../core/UIElement.h"
#include "../../resources/Fonts.h"
#include "../../utils/StringUtils.h"



class TextElement : public UIElement
{
public:
	TextElement(const sf::Font& font = Fonts::Arial, std::string string = "", sf::Vector2f size = { 160.0f, 90.0f }, sf::Vector2f position = { 0.0f, 0.0f }) :
		UIElement(size, position), m_text(font, std::move(string), static_cast<unsigned int>(size.y)), m_textSizeMode(TextSizeMode::FitToReact), m_textAlign(TextAlign::Center)
	{
		fitTextToRect();
	}


	enum class TextAlign {
		Left,
		Center,
		Right
	};

	enum class TextSizeMode
	{
		FitToReact,
		Fixed
	};


	sf::Text& text() { return m_text; }

	void setTextAlign(TextAlign align) { m_textAlign = align; centerText(); }
	TextAlign getTextAlign() const { return m_textAlign; }

	void setTextSizeMode(TextSizeMode mode) { m_textSizeMode = mode; }
	TextSizeMode getTextSizeMode() const { return m_textSizeMode; }

	void setString(std::string text) { m_text.setString(std::move(text)); fitTextToRect(); }
	std::string getString() const { return m_text.getString(); }

	void setCharacterSize(unsigned int size)
	{
		if (m_textSizeMode != TextSizeMode::Fixed)
			return;

		else
		{
			m_text.setCharacterSize(size);
			centerText();
		}
	}

	void setFont(const sf::Font& font)
	{
		m_text.setFont(font);

		if (m_textSizeMode == TextSizeMode::FitToReact)
			fitTextToRect();

		if (m_textSizeMode == TextSizeMode::Fixed)
			centerText();
	}

	const sf::Font& getFont() const { return m_text.getFont(); }

	void setOutline(float thickness, sf::Color color)
	{
		m_text.setOutlineColor(color);
		m_text.setOutlineThickness(thickness);
		if (m_textSizeMode == TextSizeMode::FitToReact)
			fitTextToRect();

		if (m_textSizeMode == TextSizeMode::Fixed)
			centerText();
	}

private:
	sf::Text m_text;
	TextAlign m_textAlign;
	TextSizeMode m_textSizeMode;

	void fitTextToRect() { ui::string::fitTextToRect(m_text, { 1.0f, 1.0f }, getLocalBounds()); }

	void onSizeChanged(sf::Vector2f ratio) override
	{
		if (m_textSizeMode == TextSizeMode::FitToReact)
			fitTextToRect();

		if (m_textSizeMode == TextSizeMode::Fixed)
			centerText();
	}

	void drawSelf(sf::RenderTarget& target, sf::RenderStates states) const override { target.draw(m_text, states); }

	void centerText()
	{
		auto textBounds = m_text.getLocalBounds();
		auto rect = getLocalBounds();

		float originY = textBounds.position.y + textBounds.size.y * 0.5f;
		float posY = rect.size.y * 0.5f;

		switch (m_textAlign)
		{
			case TextAlign::Left:
				m_text.setOrigin({ textBounds.position.x, originY });
				m_text.setPosition({ 0.0f, posY });

				break;

			case TextAlign::Center:
				m_text.setOrigin({ textBounds.position.x + textBounds.size.x * 0.5f, originY });
				m_text.setPosition({ rect.size.x * 0.5f, posY });

				break;

			case TextAlign::Right:
				m_text.setOrigin({ textBounds.position.x + textBounds.size.x, originY });
				m_text.setPosition({ rect.size.x, posY });

				break;
		}
	}
};