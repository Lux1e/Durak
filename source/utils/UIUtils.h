#pragma once
#include <SFML/Graphics.hpp>



namespace UIUtils
{
	inline sf::Color adjustColor(const sf::Color& color, uint8_t amount)
	{
		return {
		static_cast<uint8_t>(std::clamp(color.r + amount, 0, 255)),
		static_cast<uint8_t>(std::clamp(color.g + amount, 0, 255)),
		static_cast<uint8_t>(std::clamp(color.b + amount, 0, 255)),
		color.a };
	}

	inline sf::Color scaleColor(const sf::Color& color, float factor)
	{
		return {
			static_cast<uint8_t>(
				std::round(
					std::clamp(color.r * factor, 0.0f, 255.0f))
		),
					static_cast<uint8_t>(
				std::round(
					std::clamp(color.g * factor, 0.0f, 255.0f))
		),
					static_cast<uint8_t>(
				std::round(
					std::clamp(color.b * factor, 0.0f, 255.0f))
		),
		color.a };
	}
}