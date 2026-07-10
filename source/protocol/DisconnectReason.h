#pragma once
#include <cstdint>


enum class DisconnectReason : uint8_t
{
	ServerIsClosed = 0,
	ServerIsFull = 1,
	GameAlreadyStarted = 2,
	InvalidToken = 3,
	KickedByHost = 4,
	Timeout = 5,

	Quit = 6
};