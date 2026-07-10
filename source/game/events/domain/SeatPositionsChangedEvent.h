#pragma once
#include "../Event.h"



struct SeatPositionsChangedEvent final : public Event
{
	SeatPositionsChangedEvent(uint32_t playerId, int newSeatIndex) : playerId(playerId), newSeatIndex(newSeatIndex) {}

	uint32_t playerId;
	int newSeatIndex;
};