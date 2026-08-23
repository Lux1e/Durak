#pragma once
#include "../states/server/LobbyState.h"



class LobbyLogic
{
public:
	LobbyLogic()
	{
		m_lobbyState = nullptr;
	}

	LobbyLogic(LobbyState& state) : m_lobbyState(&state) {}

	void setLobbyState(LobbyState& state);
	LobbyState* getLobbyState() const;

	std::optional<DisconnectReason> isNewPlayerCanJoin() const;

	std::optional<DisconnectReason> applyPlayerConnectRequest(PlayerProfile& profile);
	void applyPlayerDisconnected(uint32_t playerId);

	bool applySeatPositionsChangeRequest(uint32_t playerId, int seatPosition, uint32_t initiatorId);
	bool applySeatPositionsSwapRequest(uint32_t firstPlayerId, uint32_t secondPlayerId, uint32_t initiatorId);

	bool applyPlayersPerGameChangeRequest(uint32_t value, uint32_t initiatorId);
	bool applyLobbyOpenState(uint32_t value, uint32_t initiatorId);

private:
	LobbyState* m_lobbyState;

	void requireState() const;
};