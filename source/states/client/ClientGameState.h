#pragma once
#include <vector>
#include "../game/ClientPlayerData.h"


class ClientGameState
{
public:

private:
	int currentPhase;

	std::vector<uint32_t> handCardsIds;
	std::vector<std::vector<uint32_t>> tableCardsIds;
	int boneyardSize;

	std::vector<ClientPlayerData> players;
};