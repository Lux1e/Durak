#pragma once
#include "../Event.h"



struct JoinGameRequestEvent final : public Event
{
	JoinGameRequestEvent(const std::string& string) : ip(string) {}

	std::string ip;
};
