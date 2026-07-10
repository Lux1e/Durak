#pragma once
#include "../Event.h"


struct ConnectionWithPlayerLostEvent final : public Event
{
	ConnectionWithPlayerLostEvent(uint32_t playerId) : playerId(playerId) {}

	uint32_t playerId;
};