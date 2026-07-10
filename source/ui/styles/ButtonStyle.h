#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>
#include "VisualComponent.h"



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

		standard.background.colors.hovered.r = static_cast<uint8_t>(std::clamp(static_cast<float>(standard.background.colors.normal.r) * backgroundHoverFactor, 0.0f, 255.0f));
		standard.background.colors.hovered.g = static_cast<uint8_t>(std::clamp(static_cast<float>(standard.background.colors.normal.g) * backgroundHoverFactor, 0.0f, 255.0f));
		standard.background.colors.hovered.b = static_cast<uint8_t>(std::clamp(static_cast<float>(standard.background.colors.normal.b) * backgroundHoverFactor, 0.0f, 255.0f));

		standard.text.colors.hovered.r = static_cast<uint8_t>(std::clamp(static_cast<float>(standard.text.colors.normal.r) * textHoverFactor, 0.0f, 255.0f));
		standard.text.colors.hovered.g = static_cast<uint8_t>(std::clamp(static_cast<float>(standard.text.colors.normal.g) * textHoverFactor, 0.0f, 255.0f));
		standard.text.colors.hovered.b = static_cast<uint8_t>(std::clamp(static_cast<float>(standard.text.colors.normal.b) * textHoverFactor, 0.0f, 255.0f));

		float backgroundPressFactor = 0.75f;
		float textPressFactor = 0.85f;

		standard.background.colors.pressed.r = static_cast<uint8_t>(std::clamp(static_cast<float>(standard.background.colors.normal.r) * backgroundPressFactor, 0.0f, 255.0f));
		standard.background.colors.pressed.g = static_cast<uint8_t>(std::clamp(static_cast<float>(standard.background.colors.normal.g) * backgroundPressFactor, 0.0f, 255.0f));
		standard.background.colors.pressed.b = static_cast<uint8_t>(std::clamp(static_cast<float>(standard.background.colors.normal.b) * backgroundPressFactor, 0.0f, 255.0f));

		standard.text.colors.pressed.r = static_cast<uint8_t>(std::clamp(static_cast<float>(standard.text.colors.normal.r) * textPressFactor, 0.0f, 255.0f));
		standard.text.colors.pressed.g = static_cast<uint8_t>(std::clamp(static_cast<float>(standard.text.colors.normal.g) * textPressFactor, 0.0f, 255.0f));
		standard.text.colors.pressed.b = static_cast<uint8_t>(std::clamp(static_cast<float>(standard.text.colors.normal.b) * textPressFactor, 0.0f, 255.0f));

		return standard;
	}
};