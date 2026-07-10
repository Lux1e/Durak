#pragma once
#include "../Event.h"
#include <optional>


struct PlayersPerGameChangedEvent final : public Event
{
	PlayersPerGameChangedEvent(uint32_t playersPerGameValue) : playersPerGame(playersPerGameValue) {}

	uint32_t playersPerGame;
};