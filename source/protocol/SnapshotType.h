#pragma once
#include <cstdint>



enum class SnapshotType : uint8_t
{
	None = 0,
	Lobby = 1,
	Game = 2,
};