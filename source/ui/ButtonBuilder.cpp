#include "ButtonBuilder.h"
#include <memory>
#include "../resources/TextureManager.h"
#include "elements/RoundedRectangleElement.h"
#include "elements/RectangleElement.h"
#include "elements/TextElement.h"
#include "elements/SpriteElement.h"



std::unique_ptr<Button> ButtonBuilder::build()
{
	std::unique_ptr<Button> buttonPtr = std::make_unique<Button>();

	auto& button = *buttonPtr.get();
	button.setSize(m_size);
	sf::Vector2f buttonSize = button.getSize();

	switch (m_type)
	{
		case ButtonType::Standard:
		case ButtonType::Rectangle:
		case ButtonType::Pilled:
		{
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
					bg->setSize({ buttonSize.x * m_style.background.sizeFactor.x, buttonSize.y * m_style.background.sizeFactor.y });
					auto& bgShape = bg->shape();
					bgShape.setFillColor(m_style.background.colors.normal);
					bgShape.setOutlineColor(m_style.background.outline.color);
					bgShape.setOutlineColor(m_style.background.outline.color);
					bgShape.setOutlineThickness(m_style.background.outline.width);
				}, backgroundVariant);


			TextElement& text = static_cast<TextElement&>(button.addChild(std::make_unique<TextElement>(*m_font, m_text)));

			text.setOrigin(text.getGeometricCenter());
			text.setPosition({ buttonSize.x / 2.0f, buttonSize.y / 2.0f });
			text.setSize({ buttonSize.x * m_style.text.sizeFactor.x, buttonSize.y * m_style.text.sizeFactor.y });

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

		case ButtonType::Dropdown:
		{
			RectangleElement& background = static_cast<RectangleElement&>(button.addChild(std::make_unique<RectangleElement>()));
			background.setSize({ buttonSize.x * m_style.background.sizeFactor.x, buttonSize.y * m_style.background.sizeFactor.y });

			sf::Vector2f maxSize = { m_size.x * 0.85f, m_size.y * 0.5f };
			sf::Vector2f minPosition = { (m_size.x - maxSize.x) * 0.5f, (m_size.y - maxSize.y) * 0.5f };
			sf::Vector2f maxPosition = { m_size.x - minPosition.x, m_size.y - minPosition.y };


			background.shape().setFillColor(m_style.background.colors.normal);
			background.shape().setOutlineColor(m_style.background.outline.color);
			background.shape().setOutlineThickness(m_style.background.outline.width);

			TextElement& string = static_cast<TextElement&>(button.addChild(std::make_unique<TextElement>(*m_font, m_text)));
			string.setSize(maxSize);
			string.setTextSizeMode(TextElement::TextSizeMode::Fixed);
			string.setTextAlign(TextElement::TextAlign::Left);
			string.setCharacterSize(string.getSize().y);
			string.setOutline(m_style.text.outline.width, m_style.text.outline.color);
			string.text().setFillColor(m_style.text.colors.normal);
			string.setPosition(minPosition);

			SpriteElement& sprite = static_cast<SpriteElement&>(button.addChild(std::make_unique<SpriteElement>(m_textureManager.getTexture("assets/textures/ui/arrow_basic_w.png"))));
			sprite.fitToSize(maxSize * 0.70f);
			sprite.setOrigin(sprite.getGeometricCenter());
			sprite.setRotation(sf::degrees(-90.0f));
			sprite.setPosition({ maxPosition.x - sprite.getSize().x * 0.5f, m_size.y * 0.5f });

			button.m_getBackgroundElement = [backgroundPtr = &background](Button&)->UIElement* { return backgroundPtr; };
			button.m_getTextElement = [stringPtr = &string](Button&)->UIElement* { return stringPtr; };
			button.m_getSpriteElement = [spritePtr = &sprite](Button&)->UIElement* { return spritePtr; };

			button.setOnHover([style = m_style, &string, &background](UIElement& element)
				{
					background.shape().setFillColor(style.background.colors.hovered);
					string.text().setFillColor(style.text.colors.hovered);
				});

			button.setOnHoverEnd([style = m_style, &string, &background](UIElement& element)
				{
					background.shape().setFillColor(style.background.colors.normal);
					string.text().setFillColor(style.text.colors.normal);
				});

			button.setOnPressed([style = m_style, &string, &background](UIInteractive& element)
				{
					background.shape().setFillColor(style.background.colors.pressed);
					string.text().setFillColor(style.text.colors.pressed);
				});

			button.setOnReleased([style = m_style, &string, &background](UIInteractive& element, bool hitTest)
				{
					if (hitTest)
					{
						background.shape().setFillColor(style.background.colors.hovered);
						string.text().setFillColor(style.text.colors.hovered);
					}

					else
					{
						background.shape().setFillColor(style.background.colors.normal);
						string.text().setFillColor(style.text.colors.normal);
					}
				});

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