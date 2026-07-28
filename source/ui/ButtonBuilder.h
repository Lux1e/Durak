#pragma once
#include "../resources/Fonts.h"
#include "widgets/Button.h"
#include "styles/ButtonType.h"
#include "styles/ButtonStyle.h"


class TextureManager;



class ButtonBuilder
{
public:
	ButtonBuilder(TextureManager& textureManager, std::string text = "", sf::Vector2f size = { 160.0f, 90.0f }, sf::Vector2f position = { 0.0f, 0.0f }, const sf::Font& font = Fonts::ArialBD, ButtonType type = ButtonType::Standard, ButtonStyle style = ButtonStyleFactory::makeStandardStyle()) :
		m_textureManager(textureManager), m_text(std::move(text)), m_size(size), m_font(&font), m_type(type), m_style(style) {}

	std::unique_ptr<Button> build();

	void setText(std::string text);
	const std::string& getText() const;

	void setFont(const sf::Font& font);
	const sf::Font* getFont() const;

	void setButtonType(ButtonType type);
	ButtonType getButtonType() const;

	void setStyle(ButtonStyle style);
	const ButtonStyle& getStyle() const;

	void setSize(sf::Vector2f size);
	sf::Vector2f getSize() const;

private:
	TextureManager& m_textureManager;
	sf::Vector2f m_size;
	std::string m_text;
	const sf::Font* m_font;
	ButtonType m_type;
	ButtonStyle m_style;
};