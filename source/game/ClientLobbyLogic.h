#pragma once
#include <cstdint>
#include <optional>
#include <vector>
#include "../protocol/ClientRole.h"


class ClientLobbyState;



struct SeatChange
{
	SeatChange(uint32_t playerId, int toSeatIndex) : playerId(playerId), toSeatIndex(toSeatIndex) {}

	uint32_t playerId;
	int toSeatIndex;
};


class ClientLobbyLogic
{
public:
	ClientLobbyLogic()
	{
		statePtr = nullptr;
	}

	void setClientLobbyState(ClientLobbyState& state);
	bool isClientLobbyState() const;

	bool applyPlayerConnected(uint32_t playerId, const std::string& nickname, ClientRole role, int seatPosition);
	void applyPlayerDisconnected(uint32_t playerId);

	bool applyPlayerNicknameChanged(uint32_t playerId, const std::string& nickname);

	bool applySeatPositionsChanged(uint32_t playerId, int seatPosition);
	bool applySeatPositionsSwapped(uint32_t firstPlayerId, uint32_t secondPlayerId);

	std::optional<SeatChange> applySeatPositionsChangeRequest(uint32_t playerId, int newSeatPosition, uint32_t initiatorId);
	std::vector<SeatChange> applySeatPositionsSwapRequest(uint32_t firstPlayerId, uint32_t secondPlayerId, uint32_t initiatorId);

	std::vector<SeatChange> applyPlayersPerGameChange(uint32_t playersPerGame);

private:
	ClientLobbyState* statePtr;

	void requireState() const;
};