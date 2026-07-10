#include "SeatPositionsState.h"



void SeatPositionsState::init()
{
	playerIdBySeatPosition.reserve(Constants::Lobby::MaxPlayersPerGame);
	seatPositionByPlayerId.reserve(Constants::Lobby::MaxPlayersPerGame);
}


std::optional<uint32_t> SeatPositionsState::assignSeat(uint32_t playerId, int32_t seatPosition)
{
	erase(playerId);

	std::optional<uint32_t> displacedPlayerId;

	auto it = playerIdBySeatPosition.find(seatPosition);
	if (it != playerIdBySeatPosition.end())
	{
		displacedPlayerId = it->second;
		erase(it->second);
	}

	playerIdBySeatPosition[seatPosition] = playerId;
	seatPositionByPlayerId[playerId] = seatPosition;

	return displacedPlayerId;
}


void SeatPositionsState::swapPlayers(uint32_t firstPlayerId, uint32_t secondPlayerId)
{
	auto firstPosition = seatPositionByPlayerId.at(firstPlayerId);
	auto secondPosition = seatPositionByPlayerId.at(secondPlayerId);

	playerIdBySeatPosition[firstPosition] = secondPlayerId;
	playerIdBySeatPosition[secondPosition] = firstPlayerId;

	seatPositionByPlayerId[firstPlayerId] = secondPosition;
	seatPositionByPlayerId[secondPlayerId] = firstPosition;
}


void SeatPositionsState::eraseFromMaps(int32_t seatPosition, uint32_t playerId)
{
	playerIdBySeatPosition.erase(seatPosition);
	seatPositionByPlayerId.erase(playerId);
}


void SeatPositionsState::erase(uint32_t playerId)
{
	auto it = seatPositionByPlayerId.find(playerId);
	if (it == seatPositionByPlayerId.end())
		return;

	int32_t seatPosition = it->second;
	eraseFromMaps(seatPosition, playerId);
}

void SeatPositionsState::erase(int32_t seatPosition)
{
	auto it = playerIdBySeatPosition.find(seatPosition);
	if (it == playerIdBySeatPosition.end())
		return;

	uint32_t playerId = it->second;
	eraseFromMaps(seatPosition, playerId);
}


const std::unordered_map<int32_t, uint32_t>& SeatPositionsState::getAllPlayersIdsBySeatPositions() const
{
	return playerIdBySeatPosition;
}


std::optional<uint32_t> SeatPositionsState::getPlayerIdBySeatPosition(int seatPosition) const
{
	auto it = playerIdBySeatPosition.find(seatPosition);
	if (it == playerIdBySeatPosition.end())
		return std::nullopt;

	return it->second;
}

std::optional<int32_t> SeatPositionsState::getSeatPositionByPlayerId(uint32_t playerId) const
{
	auto it = seatPositionByPlayerId.find(playerId);
	if (it == seatPositionByPlayerId.end())
		return std::nullopt;

	return it->second;
}