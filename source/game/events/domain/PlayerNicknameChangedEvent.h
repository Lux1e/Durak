#pragma once
#include "../Event.h"


struct PlayerNicknameChangedEvent final : public Event
{
	PlayerNicknameChangedEvent(const std::string& nickname) : playerNickname(nickname) {}

	std::string playerNickname;
};