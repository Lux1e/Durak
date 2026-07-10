#include <cassert>
#include <stdexcept>
#include "LobbyLogic.h"



void LobbyLogic::requireState() const
{
	assert(statePtr);
}


void LobbyLogic::setLobbyState(LobbyState& state)
{
	statePtr = &state;
}

bool LobbyLogic::isLobbyState() const
{
	return statePtr;
}


std::optional<DisconnectReason> LobbyLogic::isNewPlayerCanJoin() const
{
	requireState();

	if (statePtr->getPlayerProfilesInLobby().size() >= Constants::Lobby::MaxPlayersInLobby)
		return DisconnectReason::ServerIsFull;
	if (!statePtr->isLobbyOpen())
		return DisconnectReason::ServerIsClosed;

	return std::nullopt;
}


std::optional<DisconnectReason> LobbyLogic::applyPlayerConnectRequest(PlayerProfile& profile)
{
	requireState();

	auto profileOpt = statePtr->getPlayerProfileById(profile.getId());
	if (profileOpt)
		throw std::logic_error("Player already exist with this id");

	auto reasonOpt = isNewPlayerCanJoin();
	if (reasonOpt)
		return *reasonOpt;

	bool isHost = false;
	if (profile.getId() == 0)
	{
		profile.setRole(ClientRole::Host);
		isHost = true;
	}

	if (isHost)
	{
		if (!statePtr->isSeatPositionOccupied(0))
			statePtr->addPlayer(profile, 0);
		else
			statePtr->addPlayer(profile, Constants::Lobby::QueueSeat);
	}

	else
		statePtr->addPlayer(profile, Constants::Lobby::QueueSeat);

	return std::nullopt;
}

void LobbyLogic::applyPlayerDisconnected(uint32_t playerId)
{
	requireState();
	statePtr->deletePlayer(playerId);
}


bool LobbyLogic::applySeatPositionsChangeRequest(uint32_t playerId, int seatPosition, uint32_t initiatorId)
{
	requireState();

	if (!statePtr->canMoveToAnotherSeat() && initiatorId != statePtr->getHostId())
		return false;

	if (!statePtr->isSeatPositionCorrect(seatPosition))
		return false;

	if (statePtr->isSeatPositionOccupied(seatPosition))
		return false;

	auto currentSeatPositionOpt = statePtr->getSeatPositionById(playerId);
	if (!currentSeatPositionOpt)
		return false;

	if (*currentSeatPositionOpt == seatPosition)
		return false;

	if (!statePtr->getPlayerProfileById(playerId))
		return false;

	if (playerId != initiatorId)
	{
		auto initiatorOpt = statePtr->getPlayerProfileById(initiatorId);
		if (!initiatorOpt)
			throw std::logic_error("Initiator do not exist in Lobby");

		if (initiatorOpt.value()->getRole() != ClientRole::Host)
			return false;
	}

	statePtr->changePlayerSeatPosition(playerId, seatPosition);
	return true;
}

bool LobbyLogic::applySeatPositionsSwapRequest(uint32_t firstPlayerId, uint32_t secondPlayerId, uint32_t initiatorId)
{
	requireState();

	auto initiatorOpt = statePtr->getPlayerProfileById(initiatorId);
	if (!initiatorOpt)
		return false;

	if (initiatorOpt.value()->getRole() != ClientRole::Host)
		return false;

	if (firstPlayerId != initiatorId)
		if (!statePtr->getPlayerProfileById(firstPlayerId))
			return false;

	if (secondPlayerId != initiatorId)
		if (!statePtr->getPlayerProfileById(secondPlayerId))
			return false;

	auto currentFirstPlayerSeatPositionOpt = statePtr->getSeatPositionById(firstPlayerId);
	auto currentSecondPlayerSeatPositionOpt = statePtr->getSeatPositionById(secondPlayerId);

	if (!currentFirstPlayerSeatPositionOpt || !currentSecondPlayerSeatPositionOpt)
		return false;

	if (*currentFirstPlayerSeatPositionOpt == *currentSecondPlayerSeatPositionOpt)
		return false;

	statePtr->swapPlayersSeatPositions(firstPlayerId, secondPlayerId);
	return true;
}


bool LobbyLogic::applyPlayersPerGameChangeRequest(uint32_t value, uint32_t initiatorId)
{
	requireState();

	auto initiatorOpt = statePtr->getPlayerProfileById(initiatorId);
	if (!initiatorOpt)
		return false;

	if (initiatorOpt.value()->getRole() != ClientRole::Host)
		return false;

	uint32_t currentPlayersPerGame = statePtr->getPlayersPerGame();
	if (value == currentPlayersPerGame)
		return false;

	if (value > Constants::Lobby::MaxPlayersPerGame || value < Constants::Lobby::MinPlayersPerGame)
		return false;

	if (currentPlayersPerGame > value)
	{
		auto& seatPositionState = statePtr->getSeatPositionsState();
		for (int i = value; i < currentPlayersPerGame; ++i)
		{
			if (auto playerIdOpt = seatPositionState.getPlayerIdBySeatPosition(i))
				statePtr->changePlayerSeatPosition(*playerIdOpt, Constants::Lobby::QueueSeat);
		}
	}

	statePtr->setPlayersPerGame(value);
	return true;
}