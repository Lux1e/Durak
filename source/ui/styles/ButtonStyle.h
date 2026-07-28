#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>
#include "VisualComponent.h"
#include "../../utils/UIUtils.h"



struct ButtonStyle
{
	VisualComponent background;
	VisualComponent text;
};


class ButtonStyleFactory
{
public:
	static ButtonStyle makeStandardStyle(sf::Color normalBackground = { 60, 60, 70, 255 }, sf::Color normalText = { 225, 225, 225, 255 })
	{
		ButtonStyle standard;

		standard.background.sizeFactor = { 1.0f, 1.0f };
		standard.text.sizeFactor = { 0.8f, 0.5f };

		standard.background.outline.color = { 65, 65, 65, 255 };
		standard.text.outline.color = sf::Color::Black;

		standard.background.outline.width = -1.0f;
		standard.text.outline.width = 1.0f;

		standard.background.colors.normal = normalBackground;
		standard.text.colors.normal = normalText;

		float backgroundHoverFactor = 1.3f;
		int textHoverFactor = 1.3f;

		standard.background.colors.hovered = UIUtils::scaleColor(standard.background.colors.normal, backgroundHoverFactor);
		standard.text.colors.hovered = UIUtils::scaleColor(standard.text.colors.normal, textHoverFactor);

		float backgroundPressFactor = 0.75f;
		float textPressFactor = 0.85f;

		standard.background.colors.pressed = UIUtils::scaleColor(standard.background.colors.normal, backgroundPressFactor);
		standard.text.colors.pressed = UIUtils::scaleColor(standard.text.colors.normal, textPressFactor);

		return standard;
	}

	static ButtonStyle makeDropdownStyle(sf::Color normalBackground = { 110, 110, 110, 255 }, sf::Color normalText = sf::Color::Black)
	{
		ButtonStyle dropdown;

		dropdown.background.sizeFactor = { 1.0f, 1.0f };
		dropdown.text.sizeFactor = { 1.0f, 1.5f };

		dropdown.background.outline.color = { 125, 125, 125, 255 };
		dropdown.text.outline.color = sf::Color::White;

		dropdown.background.outline.width = -1.0f;
		dropdown.text.outline.width = 0.0f;

		dropdown.background.colors.normal = normalBackground;
		dropdown.text.colors.normal = normalText;

		dropdown.background.colors.hovered = { 160, 160, 170, 255 };
		dropdown.text.colors.hovered = normalText;

		dropdown.background.colors.pressed = dropdown.background.colors.hovered;
		dropdown.text.colors.pressed = dropdown.text.colors.hovered;

		return dropdown;
	}
};