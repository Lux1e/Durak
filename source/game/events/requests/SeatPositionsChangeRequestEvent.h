#pragma once
#include "../Event.h"



struct SeatPositionsChangeRequestEvent final : public Event
{
	SeatPositionsChangeRequestEvent(uint32_t playerId, int seatIndex) : id(playerId), newSeatIndex(seatIndex) {}

	uint32_t id;
	int newSeatIndex;
};