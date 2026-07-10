#include "GameState.h"
#include <assert.h>
#include "../../core/DebugLog.h"



void GameState::init(SeatPositionsState& seatPositions)
{
	m_seatPositions = seatPositions;

	_CurrentPhase = Phase::WaitingForPlayers;

	trumpSuit = std::nullopt;
	playersLowestTrumpCard = std::nullopt;

	_currentAttackerSeatPosition = -1;
	_currentDefenderSeatPosition = -1;


	table.reserve(Constants::Game::MaxTablePositions);
	table.resize(Constants::Game::MaxTablePositions);
	for (auto& pos : table)
		pos.reserve(Constants::Game::MaxCardsOnTablePosition);


	initPlayerData();
}


void GameState::initPlayerData()
{
	players.reserve(m_seatPositions.getAllPlayersIdsBySeatPositions().size());

	//to do
}



bool GameState::hasPlayer(const PlayerProfile& profile)
{
	for (const auto& p : getPlayers())
	{
		if (p.getPlayerProfile().getId() == profile.getId())
			return true;
	}

	return false;
}


void GameState::setCurrentPhase(Phase newPhase)
{
	_CurrentPhase = newPhase;
}

GameState::Phase GameState::getCurrentPhase() const
{
	return _CurrentPhase;
}

int GameState::getCurrentPhaseInt() const
{
	return static_cast<int>(_CurrentPhase);
}


const std::vector<PlayerData>& GameState::getPlayers() const
{
	return players;
}


void GameState::addCardToPlayer(int seatPosition, CardData&& card)
{
	auto optPlayerId = m_seatPositions.getPlayerIdBySeatPosition(seatPosition);
	assert(optPlayerId.has_value());
	
	auto* player = getPlayerDataBySeatPosition(seatPosition);
	assert(player != nullptr);

	player->addCard(std::move(card));
}


const PlayerData* GameState::getPlayerDataBySeatPosition(int seatPosition) const
{
	auto optPlayerId = m_seatPositions.getPlayerIdBySeatPosition(seatPosition);
	if (!optPlayerId)
		return nullptr;

	uint32_t playerId = *optPlayerId;

	for (const auto& player : players)
	{
		if (playerId == player.getPlayerProfile().getId())
			return &player;
	}

	return nullptr;
}

PlayerData* GameState::getPlayerDataBySeatPosition(int seatPosition)
{
	auto optPlayerId = m_seatPositions.getPlayerIdBySeatPosition(seatPosition);
	if (!optPlayerId)
		return nullptr;

	uint32_t playerId = *optPlayerId;

	for (auto& player : players)
	{
		if (playerId == player.getPlayerProfile().getId())
			return &player;
	}

	return nullptr;
}


int GameState::getSeatPositionByPlayerData(const PlayerData& player) const
{
	uint32_t playerId = player.getPlayerProfile().getId();
	auto optSeat = m_seatPositions.getSeatPositionByPlayerId(playerId);

	assert(optSeat.has_value());

	return *optSeat;
}


const PlayerData* GameState::getPlayerDataByPlayerId(uint32_t playerId) const
{
	for (const auto& p : players)
	{
		if (p.getId() == playerId)
			return &p;
	}

	return nullptr;
}

PlayerData* GameState::getPlayerDataByPlayerId(uint32_t playerId)
{
	for (auto& p : players)
	{
		if (p.getId() == playerId)
			return &p;
	}

	return nullptr;
}


const PlayerData* GameState::getCurrentMainAttackerPlayerData() const
{
	return getPlayerDataBySeatPosition(_currentAttackerSeatPosition);
}

PlayerData* GameState::getCurrentMainAttackerPlayerData()
{
	return getPlayerDataBySeatPosition(_currentAttackerSeatPosition);
}

const PlayerData* GameState::getCurrentDefenderPlayerData() const
{
	return getPlayerDataBySeatPosition(_currentDefenderSeatPosition);
}

PlayerData* GameState::getCurrentDefenderPlayerData()
{
	return getPlayerDataBySeatPosition(_currentDefenderSeatPosition);
}


CardData GameState::takeDeckCard()
{
	if (deck.empty())
	{
		throw std::out_of_range("No cards at this index");
		DEBUG_LOG("Cannot take card from empty deck");
	}

	CardData card = std::move(deck.back());
	deck.pop_back();
	return card;
}

CardData& GameState::getCardFromDeckByIndex(int index)
{
	return deck[index];
}

const std::vector<CardData>& GameState::getDeck() const
{
	return deck;
}

void GameState::moveInDeck(int fromIndex, int toIndex)
{
	if (fromIndex >= deck.size() || toIndex > deck.size())
		throw std::out_of_range("Invalid index");

	if (fromIndex == toIndex)
		return;

	CardData card = std::move(deck[fromIndex]);
	deck.erase(deck.begin() + fromIndex);
	deck.insert(deck.begin() + toIndex, card);
}

std::vector<const CardData*> GameState::getTableCards() const
{
	std::vector<const CardData*> allCardsOnTable;

	for (int i = 0; i < table.size(); ++i)
	{
		for (const auto& card : table[i])
		{
			allCardsOnTable.push_back(&card);
		}
	}

	return allCardsOnTable;
}

const std::vector<CardData>& GameState::getTableCardsAtPosition(int position)
{
	if (position < 0 || position >= static_cast<int>(table.size()))
		throw std::out_of_range("Invalid table position");

	return table[position];
}

void GameState::addCardToTable(int tablePosition, CardData&& card)
{
	if (tablePosition < 0 || tablePosition >= static_cast<int>(table.size()))
		throw std::out_of_range("Invalid table position");

	table[tablePosition].push_back(std::move(card));
}

CardData GameState::removeCardFromTablePosition(int tablePosition, int index)
{
	if (tablePosition < 0 || tablePosition >= static_cast<int>(table.size()))
		throw std::out_of_range("Invalid table position");

	if (table[tablePosition].empty())
		throw std::out_of_range("No cards on this table position");

	if (index < 0 || index >= static_cast<int>(table[tablePosition].size()))
		throw std::out_of_range("No card at this index");


	CardData card = std::move(table[tablePosition][index]);
	table[tablePosition].erase(table[tablePosition].begin() + index);

	return card;
}

int GameState::attackerCardsOnTableSize() const
{
	int size = 0;
	for (const auto& pos : table)
		if (pos.size() > 0)
			size++;

	return size;
}

int GameState::defenderCardsOnTableSize() const
{
	int size = 0;
	for (const auto& pos : table)
		if (pos.size() == 2)
			size++;

	return size;
}

int GameState::cardsOnTablePositionSize(int position) const
{
	if (position < 0 || position >= static_cast<int>(table.size()))
		throw std::out_of_range("Invalid table position");
	return table[position].size();
}

int GameState::cardsOnTableSize() const
{
	int number = 0;
	for (auto& pos : table)
		number += pos.size();

	return number;
}


std::vector<uint32_t> GameState::getTableCardsIds() const
{
	std::vector<uint32_t> ids;
	ids.reserve(12);

	for (const auto& cardsInPosition : table)
	{
		for (const auto& card : cardsInPosition)
		{
			ids.emplace_back(card.getId());
		}
	}

	return ids;
}

std::vector<uint32_t> GameState::getAttackerTableCardsIds() const
{
	std::vector<uint32_t> ids;
	ids.reserve(Constants::Game::MaxTablePositions);

	for (const auto& cardsInPosition : table)
	{
		if (!cardsInPosition.empty())
		{
			ids.emplace_back(cardsInPosition.front().getId());
		}
	}

	return ids;
}

std::vector<std::pair<int, uint32_t>> GameState::getDefenderTableCardsPositionsAndIds() const
{
	std::vector<std::pair<int, uint32_t>> positionsAndIds;
	positionsAndIds.reserve(Constants::Game::MaxTablePositions);

	for (int i = 0; i < Constants::Game::MaxTablePositions; ++i)
	{
		if (table[i].size() == 2)
		{
			positionsAndIds.emplace_back(i, table[i][1].getId());
		}
	}

	return positionsAndIds;
}


void GameState::addCardToBoneyard(CardData&& card)
{
	boneyard.push_back(std::move(card));
}

const std::vector<CardData>& GameState::getBoneyard() const
{
	return boneyard;
}


void GameState::setMainTrumpCardId(uint32_t cardId)
{
	mainTrumpCardId = cardId;
}

std::optional<uint32_t> GameState::getMainTrumpCardId() const
{
	return mainTrumpCardId;
}


void GameState::setPlayersLowestTrumpCard(std::pair<int, uint32_t> value) // first = playerSeatPosition, second cardId
{
	playersLowestTrumpCard = value;
}

std::optional<std::pair<int, uint32_t>> GameState::getPlayersLowestTrumpCard() const // first = playerSeatPosition, second cardId
{
	return playersLowestTrumpCard;
}


void GameState::setCurrentAttackerSeatPosition(int value)
{
	_currentAttackerSeatPosition = value;
}

const int GameState::getCurrentAttackerSeatPosition() const
{
	return _currentAttackerSeatPosition;
}


void GameState::setCurrentDefenderSeatPosition(int value)
{
	_currentDefenderSeatPosition = value;
}

const int GameState::getCurrentDefenderSeatPosition() const
{
	return _currentDefenderSeatPosition;
}


bool GameState::isDeckEmpty() const
{
	return deck.empty();
}


void GameState::setTrumpSuit(const CardData::Suit suit)
{
	trumpSuit = suit;
}

std::optional<CardData::Suit> GameState::getTrumpSuit() const
{
	return trumpSuit;
}

std::optional<std::pair<int, uint32_t>> GameState::findPlayerWithLowestTrump()
{
	if (!getTrumpSuit())
	{
		DEBUG_LOG("Trump card not exist at this point");
		throw std::runtime_error("Trump card not exist at this point");
	}


	std::optional<CardData> candidate = std::nullopt;
	int playerCandidateSeatPosition = -1;

	for (const auto& player : getPlayers())
	{
		const auto& cardCandidate = player.findLowestRankBySuit(getTrumpSuit().value());
		if (cardCandidate.has_value())
		{
			if (!candidate || cardCandidate->getRank() < candidate->getRank())
			{
				candidate = *cardCandidate;
				playerCandidateSeatPosition = getSeatPositionByPlayerData(player);
			}
		}
	}

	if (!candidate.has_value())
		return std::nullopt;

	return std::make_pair(playerCandidateSeatPosition, candidate->getId());
}