#pragma once
#include "../Event.h"


struct InvalidMoveEvent final : public Event
{
	InvalidMoveEvent(int playerSeatPosition) : playerSeatPosition(playerSeatPosition) {}

	int playerSeatPosition;
};