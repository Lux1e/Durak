#pragma once
#include "../../resources/Fonts.h"
#include "../elements/RectangleElement.h"
#include "../elements/textElement.h"



class TextBox : public UIElement
{
public:
	TextBox(std::string string = "", const sf::Font& stringFont = Fonts::ArialI, std::string placeholderString = "", const sf::Font& placeholderFont = Fonts::ArialI,
		sf::Vector2f size = { 160.0f, 90.0f }, sf::Vector2f position = { 0.0f, 0.0f }) : UIElement(size, position)
	{
		init(stringFont, string, placeholderFont, placeholderString);
	}

	void setPlaceholderRatio(sf::Vector2f ratio = { 0.8f, 0.6f });
	void setStringRatio(sf::Vector2f ratio = { 0.8f, 0.6f });

	void enterSymbol(char symbol);
	void deleteLastSymbol();

	bool isEmpty() const;

	void setPlaceholderString(std::string string);
	const std::string& getPlaceholderString() const;

	void setString(std::string text);
	void clearString();
	std::string getString() const;

	void setBoxColor(sf::Color value);
	void setPlaceholderColor(sf::Color value);
	void setStringColor(sf::Color value);

	void setBoxOutline(float thickness, sf::Color color);
	void setPlaceholderOutline(float thickness, sf::Color color);
	void setStringOutline(float thickness, sf::Color color);

private:
	void init(const sf::Font& stringFont, std::string string, const sf::Font& placeholderFont, std::string placeholderString);

	RectangleElement* m_box = nullptr;
	TextElement* m_string = nullptr;
	TextElement* m_placeholderString = nullptr;

	bool m_isEmpty;

	void updatePlaceholder();
};