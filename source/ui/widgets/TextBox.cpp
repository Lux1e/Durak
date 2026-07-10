#include "TextBox.h"
#include "Button.h"



void TextBox::init(const sf::Font& stringFont, std::string string, const sf::Font& placeholderFont, std::string placeholderString)
{
	m_isEmpty = true;

	m_box = &static_cast<RectangleElement&>(addChild(std::make_unique<RectangleElement>()));
	m_string = &static_cast<TextElement&>(addChild(std::make_unique<TextElement>(stringFont, std::move(string))));
	m_placeholderString = &static_cast<TextElement&>(addChild(std::make_unique<TextElement>(placeholderFont, std::move(placeholderString))));

	m_box->setSize(m_size);

	setPlaceholderRatio({ 0.8f, 0.6f });
	setStringRatio({ 0.8f, 0.6f });

	m_placeholderString->setOrigin(m_string->getGeometricCenter());
	m_placeholderString->setPosition(getGeometricCenter());

	m_string->setOrigin(m_string->getGeometricCenter());
	m_string->setPosition(getGeometricCenter());

	setBoxColor(sf::Color::Black);
	setPlaceholderColor(sf::Color::White);
	setStringColor(sf::Color::White);

	updatePlaceholder();
}


void TextBox::updatePlaceholder()
{
	assert(m_string);
	assert(m_placeholderString);

	if (m_string->text().getString().getSize() == 0)
	{
		m_placeholderString->setVisible(true);
		m_isEmpty = true;
	}

	else
	{
		m_placeholderString->setVisible(false);
		m_isEmpty = false;
	}
}


void TextBox::setPlaceholderRatio(sf::Vector2f ratio)
{
	m_placeholderString->setSize({ m_size.x * ratio.x, m_size.y * ratio.y });
}

void TextBox::setStringRatio(sf::Vector2f ratio)
{
	m_string->setSize({ m_size.x * ratio.x, m_size.y * ratio.y });
}


void TextBox::enterSymbol(char symbol)
{
	m_string->setString(m_string->getString() + symbol);
	updatePlaceholder();
}

void TextBox::deleteLastSymbol()
{
	if (m_isEmpty)
		return;

	std::string text = m_string->getString();
	text.pop_back();
	m_string->setString(text);

	updatePlaceholder();
}


bool TextBox::isEmpty() const
{
	return m_isEmpty;
}


void TextBox::setPlaceholderString(std::string string)
{
	m_placeholderString->setString(std::move(string));
	updatePlaceholder();
}

const std::string& TextBox::getPlaceholderString() const
{
	return m_placeholderString->getString();
}


void TextBox::setString(std::string text)
{
	m_string->setString(std::move(text));
	updatePlaceholder();
}

void TextBox::clearString()
{
	m_string->setString("");
	updatePlaceholder();
}

std::string TextBox::getString() const
{
	return m_string->getString();
}


void TextBox::setBoxColor(sf::Color value)
{
	m_box->shape().setFillColor(value);
}

void TextBox::setPlaceholderColor(sf::Color value)
{
	m_placeholderString->text().setFillColor(value);
}

void TextBox::setStringColor(sf::Color value)
{
	m_string->text().setFillColor(value);
}


void TextBox::setBoxOutline(float thickness, sf::Color color)
{
	m_box->shape().setOutlineThickness(thickness);
	m_box->shape().setOutlineColor(color);
}

void TextBox::setPlaceholderOutline(float thickness, sf::Color color)
{
	m_placeholderString->setOutline(thickness, color);
}

void TextBox::setStringOutline(float thickness, sf::Color color)
{
	m_string->setOutline(thickness, color);
}