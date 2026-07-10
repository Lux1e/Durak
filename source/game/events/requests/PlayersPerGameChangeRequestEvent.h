#pragma once
#include "../Event.h"



struct PlayersPerGameChangeRequestEvent final : public Event
{
	PlayersPerGameChangeRequestEvent(uint32_t value) : playersPerGameValue(value) {}

	uint32_t playersPerGameValue;
};