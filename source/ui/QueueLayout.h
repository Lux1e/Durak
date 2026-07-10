#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "../core/Constants.h"



class QueueLayout final
{
public:
	QueueLayout(uint32_t maxPlayersInQueue = Constants::Lobby::MaxPlayersInLobby)
	{
		positionByQueuePosition.resize(maxPlayersInQueue);
	}


	bool setPosition(uint32_t queuePosition, sf::Vector2f position)
	{
		if (queuePosition >= positionByQueuePosition.size())
			return false;

		positionByQueuePosition[queuePosition] = position;
		return true;
	}

	sf::Vector2f getPositionByIndex(uint32_t queueIndex) const
	{
		if (queueIndex >= positionByQueuePosition.size())
			return { 0,0 };

		return positionByQueuePosition[queueIndex];
	}

private:
	std::vector<sf::Vector2f> positionByQueuePosition;
};