#pragma once
#include "../Event.h"


struct TossEvent final : public Event
{
	explicit TossEvent(int playerSeatPosition, int tablePosition, uint32_t cardId) : playerSeatPosition(playerSeatPosition), tablePosition(tablePosition), cardId(cardId) {}

	int playerSeatPosition, tablePosition;
	uint32_t cardId;
};