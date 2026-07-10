#include <cassert>
#include <stdexcept>
#include "ClientLobbyLogic.h"
#include "../core/Constants.h"
#include "../states/client/ClientLobbyState.h"



void ClientLobbyLogic::requireState() const
{
	if (!statePtr)
		throw std::logic_error("statePtr is nullptr");
}


void ClientLobbyLogic::setClientLobbyState(ClientLobbyState& state)
{
	statePtr = &state;
}

bool ClientLobbyLogic::isClientLobbyState() const
{
	return statePtr;
}


bool ClientLobbyLogic::applyPlayerConnected(uint32_t playerId, const std::string& nickname, ClientRole role, int seatPosition)
{
	requireState();

	if (statePtr->getClientPlayerDataById(playerId))
		return false;

	if (statePtr->isSeatOccupied(seatPosition))
		return false;

	statePtr->addClientPlayerData(ClientPlayerData(playerId, nickname, role), seatPosition);
	return true;
}

void ClientLobbyLogic::applyPlayerDisconnected(uint32_t playerId)
{
	requireState();
	statePtr->deleteClientPlayerData(playerId);
}


bool ClientLobbyLogic::applyPlayerNicknameChanged(uint32_t playerId, const std::string& nickname)
{
	requireState();

	auto playerPtr = statePtr->getClientPlayerDataById(playerId);
	if (!playerPtr)
		return false;
	else
		playerPtr->setNickname(nickname);

	return true;
}


bool ClientLobbyLogic::applySeatPositionsChanged(uint32_t playerId, int seatPosition)
{
	requireState();

	if (!statePtr->getClientPlayerDataById(playerId))
		return false;

	if (statePtr->getPlayerIdBySeatIndex(seatPosition))
		return false;

	statePtr->movePlayerToSeatPosition(playerId, seatPosition);
	return true;
}

bool ClientLobbyLogic::applySeatPositionsSwapped(uint32_t firstPlayerId, uint32_t secondPlayerId)
{
	requireState();

	if (!statePtr->getClientPlayerDataById(firstPlayerId) || !statePtr->getClientPlayerDataById(secondPlayerId))
		return false;

	if (!statePtr->getPlayerSeatIndex(firstPlayerId) || !statePtr->getPlayerSeatIndex(secondPlayerId))
		return false;

	statePtr->swapPlayersSeatPositions(firstPlayerId, secondPlayerId);
	return true;
}


std::optional<SeatChange> ClientLobbyLogic::applySeatPositionsChangeRequest(uint32_t playerId, int newSeatPosition, uint32_t initiatorId)
{
	requireState();

	auto initiatorOpt = statePtr->getClientPlayerDataById(initiatorId);
	if (!initiatorOpt)
		return std::nullopt;

	if (initiatorId != playerId && initiatorOpt->getRole() != ClientRole::Host)
		return std::nullopt;

	if (!statePtr->canMoveToAnotherSeat() && initiatorOpt->getRole() != ClientRole::Host)
		return std::nullopt;

	auto playerPtr = statePtr->getClientPlayerDataById(playerId);
	if (!playerPtr)
		return std::nullopt;

	auto currentPlayerSeatPositionOpt = statePtr->getPlayerSeatIndex(playerId);
	if (!currentPlayerSeatPositionOpt)
		return std::nullopt;

	if (currentPlayerSeatPositionOpt == newSeatPosition)
		return std::nullopt;

	if (newSeatPosition != Constants::Lobby::QueueSeat)
	{
		auto playerInNewSeatPositionOpt = statePtr->getPlayerIdBySeatIndex(newSeatPosition);
		if (playerInNewSeatPositionOpt)
			return std::nullopt;
	}

	statePtr->movePlayerToSeatPosition(playerId, newSeatPosition);

	return SeatChange(playerId, newSeatPosition);
}

std::vector<SeatChange> ClientLobbyLogic::applySeatPositionsSwapRequest(uint32_t firstPlayerId, uint32_t secondPlayerId, uint32_t initiatorId)
{
	requireState();

	std::vector<SeatChange> vec;
	vec.reserve(2);

	auto initiator = statePtr->getClientPlayerDataById(initiatorId);
	if (!initiator)
		return vec;

	if (initiator->getRole() != ClientRole::Host)
		return vec;

	ClientPlayerData* firstPlayerPtr = nullptr;
	ClientPlayerData* secondPlayerPtr = nullptr;

	if (firstPlayerId != initiatorId)
		firstPlayerPtr = statePtr->getClientPlayerDataById(firstPlayerId);
	else
		firstPlayerPtr = initiator;

	if (secondPlayerId != initiatorId)
		secondPlayerPtr = statePtr->getClientPlayerDataById(secondPlayerId);
	else
		secondPlayerPtr = initiator;

	if (!firstPlayerPtr || !secondPlayerPtr)
		return vec;

	auto currentFirstPlayerSeatIndexOpt = statePtr->getPlayerSeatIndex(firstPlayerId);
	auto currentSecondPlayerSeatIndexOpt = statePtr->getPlayerSeatIndex(secondPlayerId);
	if ((!currentFirstPlayerSeatIndexOpt || !currentSecondPlayerSeatIndexOpt) || (*currentFirstPlayerSeatIndexOpt == *currentSecondPlayerSeatIndexOpt))
		return vec;

	statePtr->swapPlayersSeatPositions(firstPlayerId, secondPlayerId);

	vec.emplace_back(firstPlayerId, *currentSecondPlayerSeatIndexOpt);
	vec.emplace_back(secondPlayerId, *currentFirstPlayerSeatIndexOpt);

	return vec;
}


std::vector<SeatChange> ClientLobbyLogic::applyPlayersPerGameChange(uint32_t playersPerGame)
{
	requireState();

	std::vector<SeatChange> vec;
	vec.reserve(Constants::Lobby::MaxPlayersPerGame - Constants::Lobby::MinPlayersPerGame);

	uint32_t currentPlayersPerGame = statePtr->getPlayersPerGame();

	if (playersPerGame < currentPlayersPerGame)
	{
		for (uint32_t i = playersPerGame; i < currentPlayersPerGame; ++i)
		{
			if (auto playerIdOpt = statePtr->getPlayerIdBySeatIndex(i))
			{
				statePtr->movePlayerToSeatPosition(*playerIdOpt, Constants::Lobby::QueueSeat);
				vec.emplace_back(*playerIdOpt, Constants::Lobby::QueueSeat);
			}
		}
	}

	statePtr->setPlayersPerGame(playersPerGame);
	return vec;
}