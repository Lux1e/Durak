#pragma once
#include "../Event.h"


struct InvalidRequestEvent final : public Event
{
	InvalidRequestEvent(uint32_t playerId) : playerId(playerId) {};

	uint32_t playerId;
};