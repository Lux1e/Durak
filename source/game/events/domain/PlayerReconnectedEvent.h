#pragma once
#include "../Event.h"



struct PlayerReconnectedEvent final : public Event
{
	PlayerReconnectedEvent(uint32_t id) : playerId(id) {}

	uint32_t playerId;
};