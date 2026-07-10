#pragma once
#include "../Event.h"


struct ClearTableEvent final : public Event
{
	ClearTableEvent(int targetPosition) : targetPosition(targetPosition) {}

	int targetPosition;
};