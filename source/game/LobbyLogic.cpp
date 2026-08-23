#include <cassert>
#include <stdexcept>
#include "LobbyLogic.h"



void LobbyLogic::requireState() const
{
	assert(m_lobbyState);
}


void LobbyLogic::setLobbyState(LobbyState& state)
{
	m_lobbyState = &state;
}

LobbyState* LobbyLogic::getLobbyState() const
{
	return m_lobbyState;
}


std::optional<DisconnectReason> LobbyLogic::isNewPlayerCanJoin() const
{
	requireState();

	if (m_lobbyState->getPlayerProfilesInLobby().size() >= Constants::Lobby::MaxPlayersInLobby)
		return DisconnectReason::ServerIsFull;
	if (!m_lobbyState->isLobbyOpen())
		return DisconnectReason::ServerIsClosed;

	return std::nullopt;
}


std::optional<DisconnectReason> LobbyLogic::applyPlayerConnectRequest(PlayerProfile& profile)
{
	requireState();

	auto profileOpt = m_lobbyState->getPlayerProfileById(profile.getId());
	if (profileOpt)
		throw std::logic_error("Player already exist with this id");

	auto reasonOpt = isNewPlayerCanJoin();
	if (reasonOpt)
		return *reasonOpt;

	bool isHost = false;
	if (profile.getId() == 0)
	{
		profile.setRole(ClientRole::Host);
		m_lobbyState->setHostId(profile.getId());

		isHost = true;
	}

	if (isHost)
	{
		if (!m_lobbyState->isSeatPositionOccupied(0))
			m_lobbyState->addPlayer(profile, 0);
		else
			m_lobbyState->addPlayer(profile, Constants::Lobby::QueueSeat);
	}

	else
		m_lobbyState->addPlayer(profile, Constants::Lobby::QueueSeat);

	return std::nullopt;
}

void LobbyLogic::applyPlayerDisconnected(uint32_t playerId)
{
	requireState();
	m_lobbyState->deletePlayer(playerId);
}


bool LobbyLogic::applySeatPositionsChangeRequest(uint32_t playerId, int seatPosition, uint32_t initiatorId)
{
	requireState();

	if (!m_lobbyState->canMoveToAnotherSeat() && initiatorId != m_lobbyState->getHostId())
		return false;

	if (!m_lobbyState->isSeatPositionCorrect(seatPosition))
		return false;

	if (m_lobbyState->isSeatPositionOccupied(seatPosition))
		return false;

	auto currentSeatPositionOpt = m_lobbyState->getSeatPositionById(playerId);
	if (!currentSeatPositionOpt)
		return false;

	if (*currentSeatPositionOpt == seatPosition)
		return false;

	if (!m_lobbyState->getPlayerProfileById(playerId))
		return false;

	if (playerId != initiatorId)
	{
		auto initiatorOpt = m_lobbyState->getPlayerProfileById(initiatorId);
		if (!initiatorOpt)
			throw std::logic_error("Initiator do not exist in Lobby");

		if (initiatorOpt.value()->getRole() != ClientRole::Host)
			return false;
	}

	m_lobbyState->changePlayerSeatPosition(playerId, seatPosition);
	return true;
}

bool LobbyLogic::applySeatPositionsSwapRequest(uint32_t firstPlayerId, uint32_t secondPlayerId, uint32_t initiatorId)
{
	requireState();

	auto initiatorOpt = m_lobbyState->getPlayerProfileById(initiatorId);
	if (!initiatorOpt)
		return false;

	if (initiatorOpt.value()->getRole() != ClientRole::Host)
		return false;

	if (firstPlayerId != initiatorId)
		if (!m_lobbyState->getPlayerProfileById(firstPlayerId))
			return false;

	if (secondPlayerId != initiatorId)
		if (!m_lobbyState->getPlayerProfileById(secondPlayerId))
			return false;

	auto currentFirstPlayerSeatPositionOpt = m_lobbyState->getSeatPositionById(firstPlayerId);
	auto currentSecondPlayerSeatPositionOpt = m_lobbyState->getSeatPositionById(secondPlayerId);

	if (!currentFirstPlayerSeatPositionOpt || !currentSecondPlayerSeatPositionOpt)
		return false;

	if (*currentFirstPlayerSeatPositionOpt == *currentSecondPlayerSeatPositionOpt)
		return false;

	m_lobbyState->swapPlayersSeatPositions(firstPlayerId, secondPlayerId);
	return true;
}


bool LobbyLogic::applyPlayersPerGameChangeRequest(uint32_t value, uint32_t initiatorId)
{
	requireState();

	auto initiatorOpt = m_lobbyState->getPlayerProfileById(initiatorId);
	if (!initiatorOpt)
		return false;

	if (initiatorOpt.value()->getRole() != ClientRole::Host)
		return false;

	uint32_t currentPlayersPerGame = m_lobbyState->getPlayersPerGame();
	if (value == currentPlayersPerGame)
		return false;

	if (value > Constants::Lobby::MaxPlayersPerGame || value < Constants::Lobby::MinPlayersPerGame)
		return false;

	if (currentPlayersPerGame > value)
	{
		auto& seatPositionState = m_lobbyState->getSeatPositionsState();
		for (int i = value; i < currentPlayersPerGame; ++i)
		{
			if (auto playerIdOpt = seatPositionState.getPlayerIdBySeatPosition(i))
				m_lobbyState->changePlayerSeatPosition(*playerIdOpt, Constants::Lobby::QueueSeat);
		}
	}

	m_lobbyState->setPlayersPerGame(value);
	return true;
}

bool LobbyLogic::applyLobbyOpenState(uint32_t value, uint32_t initiatorId)
{
	if (value == m_lobbyState->isLobbyOpen() || initiatorId != m_lobbyState->getHostId().value())
		return false;

	m_lobbyState->setLobbyOpen(value);
	return true;
}