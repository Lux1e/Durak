#include <cassert>
#include <stdexcept>
#include "ClientLobbyState.h"
#include "../../core/Constants.h"



void ClientLobbyState::init()
{
	m_playersIdsInQueue.reserve(Constants::Lobby::MaxPlayersInLobby);
	m_clientPlayerDataById.reserve(Constants::Lobby::MaxPlayersInLobby);

	m_isLobbyOpen = true;
	m_canMoveToAnotherSeat = true;
	m_playersPerGame = Constants::Lobby::MinPlayersPerGame;
	m_rulesType = RulesType::ClassicRules;
}


bool ClientLobbyState::isValidSeatPosition(int seatPosition) const
{
	return (seatPosition < m_playersPerGame && seatPosition >= 0) || seatPosition == Constants::Lobby::QueueSeat;
}


bool ClientLobbyState::removeFromQueue(uint32_t playerId)
{
	return std::erase(m_playersIdsInQueue, playerId);
}


const std::unordered_map<uint32_t, ClientPlayerData>& ClientLobbyState::getAllPlayersById() const
{
	return m_clientPlayerDataById;
}


ClientPlayerData* ClientLobbyState::getClientPlayerDataById(uint32_t playerId)
{
	auto it = m_clientPlayerDataById.find(playerId);
	if (it == m_clientPlayerDataById.end())
		return nullptr;

	return &it->second;
}

//const ClientPlayerData* ClientLobbyState::getClientPlayerDataById(uint32_t playerId) const
//{
//	return getClientPlayerDataById(playerId);
//}

std::optional<uint32_t> ClientLobbyState::getPlayerIdBySeatIndex(int seatPosition) const
{
	if (!isValidSeatPosition(seatPosition) || seatPosition == Constants::Lobby::QueueSeat)
		return std::nullopt;

	return m_seatPositions.getPlayerIdBySeatPosition(seatPosition);
}


const std::vector<uint32_t>& ClientLobbyState::getAllPlayersIdsInQueue() const
{
	return m_playersIdsInQueue;
}

const std::vector<uint32_t> ClientLobbyState::getAllPlayersIdsInSeatPositions() const
{
	const auto& seats = m_seatPositions.getAllPlayersIdsBySeatPositions();

	std::vector<uint32_t> playerIds;
	playerIds.reserve(seats.size());

	for (const auto& [position, id] : seats)
		playerIds.emplace_back(id);

	return playerIds;
}


std::optional<uint32_t> ClientLobbyState::getPlayerIndexInQueueById(uint32_t playerId) const
{
	for (uint32_t i = 0; i < m_playersIdsInQueue.size(); ++i)
	{
		if (m_playersIdsInQueue[i] == playerId)
			return i;
	}

	return std::nullopt;
}


void ClientLobbyState::assignPlayerToSeatPosition(int32_t seatPosition, uint32_t playerId)
{
	assert(getClientPlayerDataById(playerId));
	assert(isValidSeatPosition(seatPosition));

	if (seatPosition == Constants::Lobby::QueueSeat)
		m_playersIdsInQueue.emplace_back(playerId);

	else
		m_seatPositions.assignSeat(playerId, seatPosition);
}

std::optional<int> ClientLobbyState::getPlayerSeatIndex(uint32_t playerId) const
{
	auto id = m_clientPlayerDataById.find(playerId);
	if (id == m_clientPlayerDataById.end())
		return std::nullopt;

	if (auto seatOpt = m_seatPositions.getSeatPositionByPlayerId(playerId))
		return *seatOpt;

	for (const auto& id_ : m_playersIdsInQueue)
	{
		if (id_ == playerId)
			return Constants::Lobby::QueueSeat;
	}

	return std::nullopt;
}

std::optional<int> ClientLobbyState::getPlayerSeatIndex(const ClientPlayerData& player) const
{
	return getPlayerSeatIndex(player.getId());
}


void ClientLobbyState::addClientPlayerData(ClientPlayerData playerData)
{
	uint32_t playerId = playerData.getId();

	if (m_seatPositions.getSeatPositionByPlayerId(playerId))
		m_seatPositions.erase(playerId);

	m_clientPlayerDataById.insert_or_assign(playerId, std::move(playerData));
}

void ClientLobbyState::addClientPlayerData(ClientPlayerData playerData, int seatPosition)
{
	assert(isValidSeatPosition(seatPosition));

	uint32_t playerId = playerData.getId();

	m_clientPlayerDataById.insert_or_assign(playerId, std::move(playerData));

	if (seatPosition == Constants::Lobby::QueueSeat)
		m_playersIdsInQueue.push_back(playerId);
	else
		m_seatPositions.assignSeat(playerId, seatPosition);
}

void ClientLobbyState::deleteClientPlayerData(uint32_t playerId)
{
	bool removed = false;

	removed = removeFromQueue(playerId);
	if (!removed)
		m_seatPositions.erase(playerId);

	m_clientPlayerDataById.erase(playerId);
}


bool ClientLobbyState::isSeatOccupied(int seatPosition) const
{
	if (!isValidSeatPosition(seatPosition))
		throw std::logic_error("Invalid seatPosition");

	if (seatPosition == Constants::Lobby::QueueSeat)
		return !m_playersIdsInQueue.empty();
	else
		return m_seatPositions.getPlayerIdBySeatPosition(seatPosition).has_value();
}


void ClientLobbyState::movePlayerToSeatPosition(uint32_t playerId, int seatPosition)
{
	assert(isValidSeatPosition(seatPosition));
	auto currentSeatPositionOpt = getPlayerSeatIndex(playerId);
	assert(currentSeatPositionOpt);
	assert(!m_seatPositions.getPlayerIdBySeatPosition(seatPosition));
	assert(*currentSeatPositionOpt != seatPosition);

	if (*currentSeatPositionOpt == Constants::Lobby::QueueSeat)
		removeFromQueue(playerId);
	else
		m_seatPositions.erase(playerId);

	if (seatPosition == Constants::Lobby::QueueSeat)
		m_playersIdsInQueue.push_back(playerId);
	else
		m_seatPositions.assignSeat(playerId, seatPosition);
}

void ClientLobbyState::swapPlayersSeatPositions(uint32_t firstPlayerId, uint32_t secondPlayerId)
{
	assert(firstPlayerId != secondPlayerId);

	assert(getClientPlayerDataById(firstPlayerId));
	assert(getClientPlayerDataById(secondPlayerId));

	auto firstPlayerSeatIndexOpt = getPlayerSeatIndex(firstPlayerId);
	auto secondPlayerSeatIndexOpt = getPlayerSeatIndex(secondPlayerId);

	assert(firstPlayerSeatIndexOpt);
	assert(secondPlayerSeatIndexOpt);
	assert(*firstPlayerSeatIndexOpt != *secondPlayerSeatIndexOpt);

	if (*firstPlayerSeatIndexOpt == Constants::Lobby::QueueSeat)
	{
		for (uint32_t i = 0; i < m_playersIdsInQueue.size(); ++i)
		{
			if (m_playersIdsInQueue[i] == firstPlayerId)
			{
				m_playersIdsInQueue[i] = secondPlayerId;
				break;
			}
		}

		m_seatPositions.assignSeat(firstPlayerId, *secondPlayerSeatIndexOpt);
	}

	else if (*secondPlayerSeatIndexOpt == Constants::Lobby::QueueSeat)
	{
		for (uint32_t i = 0; i < m_playersIdsInQueue.size(); ++i)
		{
			if (m_playersIdsInQueue[i] == secondPlayerId)
			{
				m_playersIdsInQueue[i] = firstPlayerId;
				break;
			}
		}

		m_seatPositions.assignSeat(firstPlayerId, *firstPlayerSeatIndexOpt);
	}

	else
		m_seatPositions.swapPlayers(firstPlayerId, secondPlayerId);
}


void ClientLobbyState::setHostId(uint32_t id)
{
	m_hostId = id;
}

uint32_t ClientLobbyState::getHostId() const
{
	assert(m_hostId);
	return m_hostId.value();
}


void ClientLobbyState::setPlayersPerGame(uint32_t value)
{
	bool isValid = value >= Constants::Lobby::MinPlayersPerGame && value <= Constants::Lobby::MaxPlayersPerGame;
	assert(isValid);

	if (value < m_playersPerGame)
	{
		for (int i = value; i < m_playersPerGame; ++i)
		{
			assert(!m_seatPositions.getPlayerIdBySeatPosition(i));
		}
	}

	m_playersPerGame = value;
}

uint32_t ClientLobbyState::getPlayersPerGame() const
{
	return m_playersPerGame;
}

void ClientLobbyState::setLobbyOpen(bool value)
{
	m_isLobbyOpen = value;
}

bool ClientLobbyState::isLobbyOpen() const
{
	return m_isLobbyOpen;
}


void ClientLobbyState::setMoveToAnotherSeat(bool value)
{
	m_canMoveToAnotherSeat = value;
}

bool ClientLobbyState::canMoveToAnotherSeat() const
{
	return m_canMoveToAnotherSeat;
}


void ClientLobbyState::setRulesType(RulesType type)
{
	m_rulesType = type;
}

RulesType ClientLobbyState::getRulesType() const
{
	return m_rulesType;
}