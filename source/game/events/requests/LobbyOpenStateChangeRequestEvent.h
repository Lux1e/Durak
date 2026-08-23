#pragma once
#include "../Event.h"



struct LobbyOpenStateChangeRequestEvent final : public Event
{
	LobbyOpenStateChangeRequestEvent(bool value) : isOpen(value) {}

	bool isOpen;
};