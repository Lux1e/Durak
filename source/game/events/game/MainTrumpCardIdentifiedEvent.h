#pragma once
#include "../Event.h"


struct MainTrumpCardIdentifiedEvent final : public Event
{
	MainTrumpCardIdentifiedEvent(uint32_t cardId) : cardId(cardId) {}

	uint32_t cardId;
};