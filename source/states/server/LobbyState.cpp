#include "LobbyState.h"
#include <assert.h>
#include <stdexcept>
#include "../../core/Constants.h"



void LobbyState::init()
{
	m_isLobbyOpen = true;
	m_canMoveToAnotherSeat = true;

	m_rulesType = RulesType::ClassicRules;
	playersPerGame = 2;

	m_playerIdsInQueue.reserve(Constants::Lobby::MaxPlayersInLobby);
}


void LobbyState::setLobbyOpen(bool value)
{
	m_isLobbyOpen = value;
}

bool LobbyState::isLobbyOpen() const
{
	return m_isLobbyOpen;
}


void LobbyState::setMoveToAnotherSeat(bool value)
{
	m_canMoveToAnotherSeat = value;
}

bool LobbyState::canMoveToAnotherSeat() const
{
	return m_canMoveToAnotherSeat;
}


void LobbyState::setRulesType(RulesType rules)
{
	m_rulesType = rules;
}

RulesType LobbyState::getRulesType() const
{
	return m_rulesType;
}


void LobbyState::setPlayersPerGame(uint32_t value)
{
	bool isValid = value <= Constants::Lobby::MaxPlayersPerGame && value >= Constants::Lobby::MinPlayersPerGame;
	assert(isValid);

	for (uint32_t i = value; i < playersPerGame; ++i)
		assert(!m_seatPositions.getPlayerIdBySeatPosition(static_cast<int>(i)));

	playersPerGame = value;
}

uint32_t LobbyState::getPlayersPerGame() const
{
	return playersPerGame;
}


const SeatPositionsState& LobbyState::getSeatPositionsState() const
{
	return m_seatPositions;
}

std::optional<int> LobbyState::getSeatPositionById(uint32_t playerId) const
{
	auto seatPositionOpt = m_seatPositions.getSeatPositionByPlayerId(playerId);
	if (seatPositionOpt)
		return *seatPositionOpt;

	for (auto& id : m_playerIdsInQueue)
	{
		if (id == playerId)
			return Constants::Lobby::QueueSeat;
	}

	return std::nullopt;
}


bool LobbyState::isSeatPositionOccupied(int seatPosition) const
{
	if (!isSeatPositionCorrect(seatPosition))
		throw std::logic_error("Invalid seatPosition");


	if (seatPosition == Constants::Lobby::QueueSeat)
	{
		if (m_playerIdsInQueue.size() >= Constants::Lobby::MaxPlayersInLobby)
			return true;
		else
			return false;
	}

	else
		return m_seatPositions.getPlayerIdBySeatPosition(seatPosition).has_value();
}


void LobbyState::changePlayerSeatPosition(uint32_t playerId, int seatPosition)
{
	assert(getPlayerProfileById(playerId));
	assert(!isSeatPositionOccupied(seatPosition));

	auto currentSeatPositionOpt = getSeatPositionById(playerId);
	assert(currentSeatPositionOpt);
	assert(*currentSeatPositionOpt != seatPosition);

	if (*currentSeatPositionOpt == Constants::Lobby::QueueSeat)
		std::erase_if(m_playerIdsInQueue, [playerId](uint32_t id) { return id == playerId; });
	else
		m_seatPositions.erase(playerId);

	if (seatPosition == Constants::Lobby::QueueSeat)
		m_playerIdsInQueue.emplace_back(playerId);
	else
		m_seatPositions.assignSeat(playerId, seatPosition);
}

void LobbyState::swapPlayersSeatPositions(uint32_t firstPlayerId, uint32_t secondPlayerId)
{
	assert(getPlayerProfileById(firstPlayerId));
	assert(getPlayerProfileById(secondPlayerId));

	auto currentFirstPlayerSeatPositionOpt = getSeatPositionById(firstPlayerId);
	auto currentSecondPlayerSeatPositionOpt = getSeatPositionById(secondPlayerId);

	assert(currentFirstPlayerSeatPositionOpt);
	assert(currentSecondPlayerSeatPositionOpt);
	assert(*currentFirstPlayerSeatPositionOpt != *currentSecondPlayerSeatPositionOpt);

	if (*currentFirstPlayerSeatPositionOpt == Constants::Lobby::QueueSeat)
	{
		for (uint32_t i = 0; i < m_playerIdsInQueue.size(); ++i)
		{
			if (m_playerIdsInQueue[i] == firstPlayerId)
			{
				m_playerIdsInQueue[i] = secondPlayerId;
				break;
			}
		}

		m_seatPositions.assignSeat(firstPlayerId, *currentSecondPlayerSeatPositionOpt);
	}

	else if (*currentSecondPlayerSeatPositionOpt == Constants::Lobby::QueueSeat)
	{
		for (uint32_t i = 0; i < m_playerIdsInQueue.size(); ++i)
		{
			if (m_playerIdsInQueue[i] == secondPlayerId)
			{
				m_playerIdsInQueue[i] = firstPlayerId;
				break;
			}
		}

		m_seatPositions.assignSeat(secondPlayerId, *currentSecondPlayerSeatPositionOpt);
	}

	else
		m_seatPositions.swapPlayers(firstPlayerId, secondPlayerId);
}


bool LobbyState::isSeatPositionCorrect(int seatPosition) const
{
	if (seatPosition == Constants::Lobby::QueueSeat)
		return true;

	if (seatPosition > playersPerGame && seatPosition < 0)
		return false;

	return true;
}


const std::vector<PlayerProfile*>& LobbyState::getPlayerProfilesInLobby() const
{
	return playerProfilesInLobby;
}

std::optional<PlayerProfile*> LobbyState::getPlayerProfileById(uint32_t playerId) const
{
	for (auto& p : playerProfilesInLobby)
		if (p->getId() == playerId)
			return p;

	return std::nullopt;
}


const std::vector<uint32_t>& LobbyState::getPlayerIdsInQueue() const
{
	return m_playerIdsInQueue;
}


void LobbyState::addPlayer(PlayerProfile& player, int seatPosition)
{
	if (!isSeatPositionCorrect(seatPosition))
		throw std::logic_error("Invalid seatPosition");

	uint32_t playerId = player.getId();
	playerProfilesInLobby.push_back(&player);

	if (seatPosition == Constants::Lobby::QueueSeat)
		m_playerIdsInQueue.push_back(playerId);
	else
		m_seatPositions.assignSeat(playerId, seatPosition);
}

void LobbyState::deletePlayer(uint32_t playerId)
{
	m_seatPositions.erase(playerId);
	std::erase_if(m_playerIdsInQueue, [playerId](uint32_t id)
		{
			return id == playerId;
		});

	std::erase_if(playerProfilesInLobby, [playerId](const PlayerProfile* player)
		{
			return player->getId() == playerId;
		});
}


bool LobbyState::hasPlayer(const PlayerProfile& profile) const
{
	for (const auto& p : playerProfilesInLobby)
	{
		if (p->getId() == profile.getId())
			return true;
	}

	return false;
}

bool LobbyState::hasPlayer(uint64_t token) const
{
	for (const auto& p : playerProfilesInLobby)
		if (p->getToken() == token)
			return true;

	return false;
}

void LobbyState::setHostId(uint32_t id)
{
	hostId = id;
}

std::optional<uint32_t> LobbyState::getHostId() const
{
	return hostId;
}


std::optional<DisconnectReason> LobbyState::isNewPlayerCanJoin()
{
	if (getPlayerProfilesInLobby().size() >= Constants::Lobby::MaxPlayersInLobby)
		return DisconnectReason::ServerIsFull;

	if (!m_isLobbyOpen)
		return DisconnectReason::ServerIsClosed;

	return std::nullopt;
}