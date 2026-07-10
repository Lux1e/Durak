#pragma once
#include "../Event.h"


struct PlayerConnectedEvent final : public Event
{
	PlayerConnectedEvent(uint32_t id, const std::string& nickname, ClientRole role, int seatPosition) :
		playerId(id), playerNickname(nickname), playerRole(role), playerSeatPosition(seatPosition) {}

	uint32_t playerId;
	const std::string playerNickname;
	ClientRole playerRole;
	int playerSeatPosition;
};