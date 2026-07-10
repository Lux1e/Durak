#pragma once
#include <unordered_map>
#include <optional>
#include "../core/IState.h"
#include "../../game/ClientPlayerData.h"
#include "../../game/RulesType.h"
#include "../SeatPositionsState.h"



class ClientLobbyState final : public IState
{
public:
	ClientLobbyState()
	{
		init();
	}


	const std::unordered_map<uint32_t, ClientPlayerData>& getAllPlayersById() const;

	ClientPlayerData* getClientPlayerDataById(uint32_t playerId);
	//const ClientPlayerData* getClientPlayerDataById(uint32_t playerId) const;

	std::optional<uint32_t> getPlayerIdBySeatIndex(int seatPosition) const;

	const std::vector<uint32_t>& getAllPlayersIdsInQueue() const;
	const std::vector<uint32_t> getAllPlayersIdsInSeatPositions() const;

	std::optional<uint32_t> getPlayerIndexInQueueById(uint32_t playerId) const;

	void assignPlayerToSeatPosition(int32_t seatPosition, uint32_t playerId);
	std::optional<int> getPlayerSeatIndex(uint32_t playerId) const;
	std::optional<int> getPlayerSeatIndex(const ClientPlayerData& player) const;

	void addClientPlayerData(ClientPlayerData playerData);
	void addClientPlayerData(ClientPlayerData playerData, int seatPosition);
	void deleteClientPlayerData(uint32_t playerId);

	bool isSeatOccupied(int seatPosition) const;

	void movePlayerToSeatPosition(uint32_t playerId, int seatPosition);
	void swapPlayersSeatPositions(uint32_t firstPlayerId, uint32_t secondPlayerId);

	void setHostId(uint32_t id);
	uint32_t getHostId() const;

	void setPlayersPerGame(uint32_t value);
	uint32_t getPlayersPerGame() const;

	void setLobbyOpen(bool value);
	bool isLobbyOpen() const;

	void setMoveToAnotherSeat(bool value);
	bool canMoveToAnotherSeat() const;

	void setRulesType(RulesType type);
	RulesType getRulesType() const;

private:
	std::unordered_map<uint32_t, ClientPlayerData> m_clientPlayerDataById;

	std::vector<uint32_t> m_playersIdsInQueue;
	SeatPositionsState m_seatPositions;

	std::optional<uint32_t> m_hostId;

	RulesType m_rulesType;
	uint32_t m_playersPerGame;
	bool m_isLobbyOpen;
	bool m_canMoveToAnotherSeat;


	void init();


	bool isValidSeatPosition(int seatPosition) const;

	bool removeFromQueue(uint32_t playerId);
};