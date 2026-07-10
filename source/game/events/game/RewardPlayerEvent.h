#pragma once
#include "../Event.h"


struct RewardPlayerEvent final : public Event
{
	RewardPlayerEvent(int playerSeatPosition) : playerSeatPosition(playerSeatPosition) {}

	int playerSeatPosition;
};