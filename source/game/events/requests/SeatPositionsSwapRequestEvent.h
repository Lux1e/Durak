#pragma once
#include "../Event.h"



struct SeatPositionsSwapRequestEvent final : public Event
{
	SeatPositionsSwapRequestEvent(uint32_t firstPlayerId, uint32_t secondPlayerId) : firstPlayerId(firstPlayerId), secondPlayerId(secondPlayerId) {}

	uint32_t firstPlayerId;
	uint32_t secondPlayerId;
};