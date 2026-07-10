#pragma once
#include <unordered_map>
#include <functional>
#include "../core/IState.h"
#include "../../core/Trackable.h"
#include "../../game/RulesType.h"
#include "../../game/PlayerProfile.h"
#include "../SeatPositionsState.h"
#include "../../protocol/DisconnectReason.h"



class LobbyState final : public IState, public Trackable
{
public:
	LobbyState()
	{
		init();
	}


	void setLobbyOpen(bool value);
	bool isLobbyOpen() const;

	void setMoveToAnotherSeat(bool value);
	bool canMoveToAnotherSeat() const;

	void setRulesType(RulesType rules);
	RulesType getRulesType() const;

	void setPlayersPerGame(uint32_t value);
	uint32_t getPlayersPerGame() const;

	const SeatPositionsState& getSeatPositionsState() const;
	std::optional<int> getSeatPositionById(uint32_t playerId) const;

	bool isSeatPositionCorrect(int seatPosition) const;
	bool isSeatPositionOccupied(int seatPosition) const;

	void changePlayerSeatPosition(uint32_t playerId, int seatPosition);
	void swapPlayersSeatPositions(uint32_t firstPlayerId, uint32_t secondPlayerId);

	const std::vector<PlayerProfile*>& getPlayerProfilesInLobby() const;
	std::optional<PlayerProfile*> getPlayerProfileById(uint32_t playerId) const;

	const std::vector<uint32_t>& getPlayerIdsInQueue() const;

	void addPlayer(PlayerProfile& player, int seatPosition);
	void deletePlayer(uint32_t playerId);

	bool hasPlayer(const PlayerProfile& profile) const;
	bool hasPlayer(uint64_t token) const;

	std::optional<uint32_t> getHostId() const;

private:
	SeatPositionsState m_seatPositions;

	bool m_isLobbyOpen;
	bool m_canMoveToAnotherSeat;

	std::vector<PlayerProfile*> playerProfilesInLobby;
	std::vector<uint32_t> m_playerIdsInQueue;

	RulesType m_rulesType;
	uint32_t playersPerGame;
	std::optional<uint32_t> hostId;


	void init();

	std::optional<DisconnectReason> isNewPlayerCanJoin();
};