#pragma once
#include "../Event.h"


struct ShowPlayerTrumpCardEvent final : public Event
{
	ShowPlayerTrumpCardEvent(int playerSeatPosition, uint32_t cardId) : playerSeatPosition(playerSeatPosition), cardId(cardId) {}

	int playerSeatPosition;
	uint32_t cardId;
};