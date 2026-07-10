#pragma once
#include "../Event.h"


struct DealCardEvent final : public Event
{
	DealCardEvent(int playerSeatPosition, uint32_t cardId) : playerSeatPosition(playerSeatPosition), cardId (cardId) {}

	int playerSeatPosition;
	uint32_t cardId;
};