#include <cassert>
#include <stdexcept>
#include "ClientLobbyLogic.h"
#include "../core/Constants.h"
#include "../states/client/ClientLobbyState.h"



void ClientLobbyLogic::requireState() const
{
	if (!m_clientLobbyState)
		throw std::logic_error("statePtr is nullptr");
}


void ClientLobbyLogic::setClientLobbyState(ClientLobbyState& state)
{
	m_clientLobbyState = &state;
}

bool ClientLobbyLogic::isClientLobbyState() const
{
	return m_clientLobbyState;
}


bool ClientLobbyLogic::applyPlayerConnected(uint32_t playerId, const std::string& nickname, ClientRole role, int seatPosition)
{
	requireState();

	if (m_clientLobbyState->getClientPlayerDataById(playerId))
		return false;

	if (m_clientLobbyState->isSeatOccupied(seatPosition))
		return false;

	m_clientLobbyState->addClientPlayerData(ClientPlayerData(playerId, nickname, role), seatPosition);
	return true;
}

void ClientLobbyLogic::applyPlayerDisconnected(uint32_t playerId)
{
	requireState();
	m_clientLobbyState->deleteClientPlayerData(playerId);
}


bool ClientLobbyLogic::applyPlayerNicknameChanged(uint32_t playerId, const std::string& nickname)
{
	requireState();

	auto playerPtr = m_clientLobbyState->getClientPlayerDataById(playerId);
	if (!playerPtr)
		return false;
	else
		playerPtr->setNickname(nickname);

	return true;
}


bool ClientLobbyLogic::applySeatPositionsChanged(uint32_t playerId, int seatPosition)
{
	requireState();

	if (!m_clientLobbyState->getClientPlayerDataById(playerId))
		return false;

	if (m_clientLobbyState->getPlayerIdBySeatIndex(seatPosition))
		return false;

	m_clientLobbyState->movePlayerToSeatPosition(playerId, seatPosition);
	return true;
}

bool ClientLobbyLogic::applySeatPositionsSwapped(uint32_t firstPlayerId, uint32_t secondPlayerId)
{
	requireState();

	if (!m_clientLobbyState->getClientPlayerDataById(firstPlayerId) || !m_clientLobbyState->getClientPlayerDataById(secondPlayerId))
		return false;

	if (!m_clientLobbyState->getPlayerSeatIndex(firstPlayerId) || !m_clientLobbyState->getPlayerSeatIndex(secondPlayerId))
		return false;

	m_clientLobbyState->swapPlayersSeatPositions(firstPlayerId, secondPlayerId);
	return true;
}


std::optional<SeatChange> ClientLobbyLogic::applySeatPositionsChangeRequest(uint32_t playerId, int newSeatPosition, uint32_t initiatorId)
{
	requireState();

	auto initiatorOpt = m_clientLobbyState->getClientPlayerDataById(initiatorId);
	if (!initiatorOpt)
		return std::nullopt;

	if (initiatorId != playerId && initiatorOpt->getRole() != ClientRole::Host)
		return std::nullopt;

	if (!m_clientLobbyState->canMoveToAnotherSeat() && initiatorOpt->getRole() != ClientRole::Host)
		return std::nullopt;

	auto playerPtr = m_clientLobbyState->getClientPlayerDataById(playerId);
	if (!playerPtr)
		return std::nullopt;

	auto currentPlayerSeatPositionOpt = m_clientLobbyState->getPlayerSeatIndex(playerId);
	if (!currentPlayerSeatPositionOpt)
		return std::nullopt;

	if (currentPlayerSeatPositionOpt == newSeatPosition)
		return std::nullopt;

	if (newSeatPosition != Constants::Lobby::QueueSeat)
	{
		auto playerInNewSeatPositionOpt = m_clientLobbyState->getPlayerIdBySeatIndex(newSeatPosition);
		if (playerInNewSeatPositionOpt)
			return std::nullopt;
	}

	m_clientLobbyState->movePlayerToSeatPosition(playerId, newSeatPosition);

	return SeatChange(playerId, newSeatPosition);
}

std::vector<SeatChange> ClientLobbyLogic::applySeatPositionsSwapRequest(uint32_t firstPlayerId, uint32_t secondPlayerId, uint32_t initiatorId)
{
	requireState();

	std::vector<SeatChange> vec;
	vec.reserve(2);

	auto initiator = m_clientLobbyState->getClientPlayerDataById(initiatorId);
	if (!initiator)
		return vec;

	if (initiator->getRole() != ClientRole::Host)
		return vec;

	ClientPlayerData* firstPlayerPtr = nullptr;
	ClientPlayerData* secondPlayerPtr = nullptr;

	if (firstPlayerId != initiatorId)
		firstPlayerPtr = m_clientLobbyState->getClientPlayerDataById(firstPlayerId);
	else
		firstPlayerPtr = initiator;

	if (secondPlayerId != initiatorId)
		secondPlayerPtr = m_clientLobbyState->getClientPlayerDataById(secondPlayerId);
	else
		secondPlayerPtr = initiator;

	if (!firstPlayerPtr || !secondPlayerPtr)
		return vec;

	auto currentFirstPlayerSeatIndexOpt = m_clientLobbyState->getPlayerSeatIndex(firstPlayerId);
	auto currentSecondPlayerSeatIndexOpt = m_clientLobbyState->getPlayerSeatIndex(secondPlayerId);
	if ((!currentFirstPlayerSeatIndexOpt || !currentSecondPlayerSeatIndexOpt) || (*currentFirstPlayerSeatIndexOpt == *currentSecondPlayerSeatIndexOpt))
		return vec;

	m_clientLobbyState->swapPlayersSeatPositions(firstPlayerId, secondPlayerId);

	vec.emplace_back(firstPlayerId, *currentSecondPlayerSeatIndexOpt);
	vec.emplace_back(secondPlayerId, *currentFirstPlayerSeatIndexOpt);

	return vec;
}


std::vector<SeatChange> ClientLobbyLogic::applyPlayersPerGameChange(uint32_t playersPerGame)
{
	requireState();

	std::vector<SeatChange> vec;
	vec.reserve(Constants::Lobby::MaxPlayersPerGame - Constants::Lobby::MinPlayersPerGame);

	uint32_t currentPlayersPerGame = m_clientLobbyState->getPlayersPerGame();

	if (playersPerGame < currentPlayersPerGame)
	{
		for (uint32_t i = playersPerGame; i < currentPlayersPerGame; ++i)
		{
			if (auto playerIdOpt = m_clientLobbyState->getPlayerIdBySeatIndex(i))
			{
				m_clientLobbyState->movePlayerToSeatPosition(*playerIdOpt, Constants::Lobby::QueueSeat);
				vec.emplace_back(*playerIdOpt, Constants::Lobby::QueueSeat);
			}
		}
	}

	m_clientLobbyState->setPlayersPerGame(playersPerGame);
	return vec;
}

bool ClientLobbyLogic::applyLobbyOpenState(bool isOpen)
{
	requireState();

	if (isOpen == m_clientLobbyState->isLobbyOpen())
		return false;

	m_clientLobbyState->setLobbyOpen(isOpen);
	return true;
}