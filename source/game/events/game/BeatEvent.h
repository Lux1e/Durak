#pragma once
#include "../Event.h"


struct BeatEvent final : public Event
{
	BeatEvent(int playerSeatPosition, int tablePosition, uint32_t cardId) : playerSeatPosition(playerSeatPosition), tablePosition(tablePosition), cardId(cardId) {}

	int playerSeatPosition, tablePosition;
	uint32_t cardId;
};