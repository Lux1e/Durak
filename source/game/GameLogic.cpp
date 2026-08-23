#include "GameLogic.h"
#include <stdexcept>
#include "../core/DebugLog.h"



void GameLogic::requireState() const
{
	if (!m_gameState)
		throw std::logic_error("statePtr is nullptr in GameLogic");
}


void GameLogic::shuffleDeck()
{
	//to do
}

uint32_t GameLogic::dealOneCard(int playerSeatPosition)
{
	requireState();

	if (m_gameState->getDeck().empty())
	{
		DEBUG_LOG("Cannot deal card from empty deck");
		throw std::runtime_error("Cannot deal card from empty deck");

		return -1;
	}

	const auto& player = m_gameState->getPlayerDataBySeatPosition(playerSeatPosition);
	if (player == nullptr)
	{
		std::string seatPos = std::to_string(playerSeatPosition);
		throw std::runtime_error("No player exist at seat position :" + seatPos);

		return -1;
	}


	auto card = m_gameState->takeDeckCard();
	uint32_t cardId = card.getId();

	player->addCard(std::move(card));

	return cardId;
}


uint32_t GameLogic::identifyTrumpCard()
{
	requireState();

	CardData& mainTrumpCard = m_gameState->getCardFromDeckByIndex(m_gameState->getDeck().size() - 1);
	uint32_t cardId = mainTrumpCard.getId();

	m_gameState->setMainTrumpCardId(cardId);
	m_gameState->setTrumpSuit(mainTrumpCard.getSuit());

	m_gameState->moveInDeck(m_gameState->getDeck().size() - 1, 0);

	return cardId;
}


std::optional<std::pair<int, uint32_t>> GameLogic::identifyFirstMove()
{
	requireState();

	if (m_gameState->getPlayersLowestTrumpCard().has_value())
		return m_gameState->getPlayersLowestTrumpCard();


	auto candidate = m_gameState->findPlayerWithLowestTrump();

	if (candidate)
		m_gameState->setPlayersLowestTrumpCard(*candidate);

	return candidate;
}


bool GameLogic::toss(int playerSeatPosition, int tablePosition, uint32_t cardId)
{
	requireState();

	auto player = m_gameState->getPlayerDataBySeatPosition(playerSeatPosition);
	auto card = player->takeCardById(cardId);

	if (card == std::nullopt || m_gameState->cardsOnTablePositionSize(tablePosition) > 0)
		return false;


	m_gameState->addCardToTable(tablePosition, std::move(card.value()));
	return true;
}

bool GameLogic::beat(int playerSeatPosition, int tablePosition, uint32_t cardId)
{
	requireState();

	auto player = m_gameState->getPlayerDataBySeatPosition(playerSeatPosition);
	auto card = player->takeCardById(cardId);

	if (card == std::nullopt || m_gameState->cardsOnTablePositionSize(tablePosition) != 1)
		return false;


	m_gameState->addCardToTable(tablePosition, std::move(card.value()));
	return true;
}


void GameLogic::moveTableCardsToBoneyard()
{
	requireState();

	for (int i = 0; i < m_gameState->attackerCardsOnTableSize(); ++i)
	{
		for (int j = m_gameState->cardsOnTablePositionSize(i) - 1; j >= 0; --j)
		{
			m_gameState->addCardToBoneyard(m_gameState->removeCardFromTablePosition(i, j));
		}
	}
}


void GameLogic::moveTableCardsToHand(int seatPosition)
{
	requireState();

	for (int i = 0; i < m_gameState->attackerCardsOnTableSize(); ++i)
	{
		for (int j = m_gameState->cardsOnTablePositionSize(i) - 1; j >= 0; --j)
		{
			m_gameState->addCardToPlayer(seatPosition, m_gameState->removeCardFromTablePosition(i, j));
		}
	}
}