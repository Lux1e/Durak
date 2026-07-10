#pragma once
#include <unordered_map>
#include <optional>
#include "../core/Constants.h"



class SeatPositionsState final
{
public:
	SeatPositionsState()
	{
		init();
	}


	std::optional<uint32_t> assignSeat(uint32_t playerId, int32_t seatPosition);
	void swapPlayers(uint32_t firstPlayerId, uint32_t secondPlayerId);

	void erase(uint32_t playerId);
	void erase(int32_t seatPosition);

	const std::unordered_map<int32_t, uint32_t>& getAllPlayersIdsBySeatPositions() const;

	std::optional<uint32_t> getPlayerIdBySeatPosition(int seatPosition) const;
	std::optional<int32_t> getSeatPositionByPlayerId(uint32_t playerId) const;

private:
	std::unordered_map<int32_t, uint32_t> playerIdBySeatPosition;
	std::unordered_map<uint32_t, int32_t> seatPositionByPlayerId;


	void init();


	void eraseFromMaps(int32_t seatPosition, uint32_t playerId);
};