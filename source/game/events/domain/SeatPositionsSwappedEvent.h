#pragma once
#include "../Event.h"


struct SeatPositionsSwappedEvent final : public Event
{
	SeatPositionsSwappedEvent(uint32_t firstPlayerId, uint32_t secondPlayerId) : firstPlayerId(firstPlayerId), secondPlayerId(secondPlayerId) {}

	uint32_t firstPlayerId;
	uint32_t secondPlayerId;
};