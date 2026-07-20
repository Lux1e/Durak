#include <memory>
#include "ButtonBuilder.h"
#include "elements/RoundedRectangleElement.h"
#include "elements/RectangleElement.h"
#include "elements/TextElement.h"



std::unique_ptr<Button> ButtonBuilder::build()
{
	std::unique_ptr<Button> buttonPtr = std::make_unique<Button>();

	switch (m_type)
	{
		case ButtonType::Standard:
		case ButtonType::Rectangle:
		case ButtonType::Pilled:
		{
			auto& button = *buttonPtr.get();
			button.setSize(m_size);

			std::variant<RoundedRectangleElement*, RectangleElement*> backgroundVariant;
			UIElement* backgroundPtr = nullptr;

			if (m_type == ButtonType::Standard || m_type == ButtonType::Pilled)
			{
				backgroundPtr = &static_cast<RoundedRectangleElement&>(button.addChild(std::make_unique<RoundedRectangleElement>()));
				backgroundVariant = static_cast<RoundedRectangleElement*>(backgroundPtr);

				if (m_type == ButtonType::Pilled)
				{
					std::visit([](const auto& bg)
						{
							if constexpr (std::is_same_v<std::decay_t<decltype(bg)>, RoundedRectangleElement*>)
								bg->shape().setCornerRatio(0.5f);
						}, backgroundVariant);
				}
			}

			else if (m_type == ButtonType::Rectangle)
			{
				backgroundPtr = &static_cast<RectangleElement&>(button.addChild(std::make_unique<RectangleElement>()));
				backgroundVariant = static_cast<RectangleElement*>(backgroundPtr);
			}

			std::visit([&](const auto& bg)
				{
					bg->setSize({ m_size.x * m_style.background.sizeFactor.x, m_size.y * m_style.background.sizeFactor.y });
					auto& bgShape = bg->shape();
					bgShape.setFillColor(m_style.background.colors.normal);
					bgShape.setOutlineColor(m_style.background.outline.color);
					bgShape.setOutlineColor(m_style.background.outline.color);
					bgShape.setOutlineThickness(m_style.background.outline.width);
				}, backgroundVariant);


			TextElement& text = static_cast<TextElement&>(button.addChild(std::make_unique<TextElement>(*m_font, m_text)));

			text.setOrigin(text.getGeometricCenter());
			text.setPosition({ m_size.x / 2.0f, m_size.y / 2.0f });
			text.setSize({ m_size.x * m_style.text.sizeFactor.x, m_size.y * m_style.text.sizeFactor.y });

			auto& textShape = text.text();
			textShape.setFillColor(m_style.text.colors.normal);
			textShape.setOutlineColor(m_style.text.outline.color);
			textShape.setOutlineThickness(m_style.text.outline.width);


			button.setOnHover([&textShape, background = backgroundVariant, style = m_style](UIInteractive& element)
				{
					std::visit([&](const auto& bg) { bg->shape().setFillColor(style.background.colors.hovered); }, background);
					textShape.setFillColor(style.text.colors.hovered);
				});

			button.setOnHoverEnd([&textShape, background = backgroundVariant, style = m_style](UIInteractive& element)
				{
					std::visit([&](const auto& bg) { bg->shape().setFillColor(style.background.colors.normal); }, background);
					textShape.setFillColor(style.text.colors.normal);
				});

			button.setOnPressed([&textShape, background = backgroundVariant, style = m_style](UIInteractive& element)
				{
					std::visit([&](const auto& bg) { bg->shape().setFillColor(style.background.colors.pressed); }, background);
					textShape.setFillColor(style.text.colors.pressed);
				});

			button.setOnReleased([&textShape, background = backgroundVariant, style = m_style](UIInteractive& element, bool hitTest)
				{
					if (hitTest)
					{
						std::visit([&](const auto& bg) { bg->shape().setFillColor(style.background.colors.hovered); }, background);
						textShape.setFillColor(style.text.colors.hovered);
					}

					else
					{
						std::visit([&](const auto& bg) { bg->shape().setFillColor(style.background.colors.normal); }, background);
						textShape.setFillColor(style.text.colors.normal);
					}
				});


			button.m_getBackgroundElement = [backgroundPtr](Button&) -> UIElement* { return backgroundPtr; };
			button.m_getTextElement = [&text](Button&) -> UIElement* { return &text; };

			break;
		}
	}

	return buttonPtr;
}


void ButtonBuilder::setText(std::string text)
{
	m_text = std::move(text);
}

const std::string& ButtonBuilder::getText() const
{
	return m_text;
}


void ButtonBuilder::setFont(const sf::Font& font)
{
	m_font = &font;
}

const sf::Font* ButtonBuilder::getFont() const
{
	return m_font;
}


void ButtonBuilder::setButtonType(ButtonType type)
{
	m_type = type;
}

ButtonType ButtonBuilder::getButtonType() const
{
	return m_type;
}


void ButtonBuilder::setStyle(ButtonStyle style)
{
	m_style = std::move(style);
}

const ButtonStyle& ButtonBuilder::getStyle() const
{
	return m_style;
}


void ButtonBuilder::setSize(sf::Vector2f size)
{
	m_size = size;
}

sf::Vector2f ButtonBuilder::getSize() const
{
	return m_size;
}