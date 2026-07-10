#pragma once
#include "../Event.h"
#include "../../../protocol/DisconnectReason.h"


struct PlayerDisconnectedEvent final : public Event
{
	PlayerDisconnectedEvent(uint32_t playerId, DisconnectReason reason) : playerId(playerId), reason(reason) {}

	uint32_t playerId;
	DisconnectReason reason;
};