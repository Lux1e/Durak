#pragma once
#include "../states/client/ClientLobbyState.h"
#include "../game/ClientPlayerData.h"
#include "../protocol/ClientRole.h"



struct ClientLobbyStateBuilder
{
	ClientLobbyStateBuilder()
	{
		state = std::make_unique<ClientLobbyState>();

		state->setPlayersPerGame(Constants::Lobby::MaxPlayersPerGame);

		state->addClientPlayerData(ClientPlayerData(0, "Player0", ClientRole::Host), 0);
		state->addClientPlayerData(ClientPlayerData(1, "Player1", ClientRole::Regular), 1);
		state->addClientPlayerData(ClientPlayerData(2, "Player2", ClientRole::Regular), 2);
		state->addClientPlayerData(ClientPlayerData(3, "Player3", ClientRole::Regular), 3);
		state->addClientPlayerData(ClientPlayerData(4, "Player4", ClientRole::Regular), -1);
		state->addClientPlayerData(ClientPlayerData(5, "Player5", ClientRole::Regular), -1);
		state->addClientPlayerData(ClientPlayerData(6, "Player6", ClientRole::Regular), -1);
		state->addClientPlayerData(ClientPlayerData(7, "Player7", ClientRole::Regular), -1);
	}

	std::unique_ptr<ClientLobbyState> state;
};