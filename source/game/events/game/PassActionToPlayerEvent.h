#pragma once
#include "../Event.h"


struct PassActionToPlayerEvent final : public Event
{
	PassActionToPlayerEvent(int playerSeatPosition, bool value) : playerSeatPosition(playerSeatPosition), value(value) {}

	int playerSeatPosition;
	bool value;
};