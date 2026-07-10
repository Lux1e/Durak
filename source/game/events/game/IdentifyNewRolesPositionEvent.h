#pragma once
#include "../Event.h"


struct IdentifyNewRolesPositionEvent final : public Event
{
	IdentifyNewRolesPositionEvent(int mainAttackerPlayerSeatPosition, int defenderSeatPosition) :
		mainAttackerPlayerSeatPosition(mainAttackerPlayerSeatPosition), defenderSeatPosition(defenderSeatPosition) {}

	int mainAttackerPlayerSeatPosition, defenderSeatPosition;
};