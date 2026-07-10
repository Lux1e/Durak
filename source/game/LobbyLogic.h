#pragma once
#include "../states/server/LobbyState.h"



class LobbyLogic
{
public:
	LobbyLogic()
	{
		statePtr = nullptr;
	}

	LobbyLogic(LobbyState& state) : statePtr(&state) {}

	void setLobbyState(LobbyState& state);
	bool isLobbyState() const;

	std::optional<DisconnectReason> isNewPlayerCanJoin() const;

	std::optional<DisconnectReason> applyPlayerConnectRequest(PlayerProfile& profile);
	void applyPlayerDisconnected(uint32_t playerId);

	bool applySeatPositionsChangeRequest(uint32_t playerId, int seatPosition, uint32_t initiatorId);
	bool applySeatPositionsSwapRequest(uint32_t firstPlayerId, uint32_t secondPlayerId, uint32_t initiatorId);

	bool applyPlayersPerGameChangeRequest(uint32_t value, uint32_t initiatorId);
private:
	LobbyState* statePtr;

	void requireState() const;
};