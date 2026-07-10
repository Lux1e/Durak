#pragma once
#include <cstdint>


enum class SessionPhase : uint8_t
{
	None = 0,
	LobbyPhase = 1,
	GamePhase = 2
};