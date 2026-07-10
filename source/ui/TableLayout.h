#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>
#include "../core/Constants.h"



class TableLayout final
{
public:
	TableLayout(uint32_t positionsSize = Constants::Lobby::MinPlayersPerGame)
	{
		m_positionBySeatPosition.reserve(Constants::Lobby::MaxPlayersPerGame);
		m_positionBySeatPosition.resize(positionsSize);
	}

	bool setPosition(uint32_t seatPosition, sf::Vector2f position)
	{
		if (seatPosition >= m_positionBySeatPosition.size())
			return false;

		m_positionBySeatPosition[seatPosition] = position;
		return true;
	}

	std::optional<sf::Vector2f> getPosition(uint32_t seatPosition) const
	{
		if (seatPosition >= m_positionBySeatPosition.size())
			return std::nullopt;

		return m_positionBySeatPosition[seatPosition];
	}

	void clearAndResize(uint32_t value)
	{
		m_positionBySeatPosition.clear();
		m_positionBySeatPosition.resize(value);
	}

private:
	std::vector<sf::Vector2f> m_positionBySeatPosition;
};