#pragma once
#include "../Event.h"



struct LobbyOpenStateChangedEvent final : public Event
{
	LobbyOpenStateChangedEvent(bool value) : isOpen(value) {}

	bool isOpen;
};