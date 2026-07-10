#include "GameLogic.h"
#include <stdexcept>
#include "../core/DebugLog.h"



void GameLogic::requireState() const
{
	if (!statePtr)
		throw std::logic_error("statePtr is nullptr in GameLogic");
}


uint32_t GameLogic::dealOneCard(int playerSeatPosition)
{
	requireState();

	if (statePtr->getDeck().empty())
	{
		DEBUG_LOG("Cannot deal card from empty deck");
		throw std::runtime_error("Cannot deal card from empty deck");

		return -1;
	}

	const auto& player = statePtr->getPlayerDataBySeatPosition(playerSeatPosition);
	if (player == nullptr)
	{
		std::string seatPos = std::to_string(playerSeatPosition);
		throw std::runtime_error("No player exist at seat position :" + seatPos);

		return -1;
	}


	auto card = statePtr->takeDeckCard();
	uint32_t cardId = card.getId();

	player->addCard(std::move(card));

	return cardId;
}


uint32_t GameLogic::identifyTrumpCard()
{
	requireState();

	CardData& mainTrumpCard = statePtr->getCardFromDeckByIndex(statePtr->getDeck().size() - 1);
	uint32_t cardId = mainTrumpCard.getId();

	statePtr->setMainTrumpCardId(cardId);
	statePtr->setTrumpSuit(mainTrumpCard.getSuit());

	statePtr->moveInDeck(statePtr->getDeck().size() - 1, 0);

	return cardId;
}


std::optional<std::pair<int, uint32_t>> GameLogic::identifyFirstMove()
{
	requireState();

	if (statePtr->getPlayersLowestTrumpCard().has_value())
		return statePtr->getPlayersLowestTrumpCard();


	auto candidate = statePtr->findPlayerWithLowestTrump();

	if (candidate)
		statePtr->setPlayersLowestTrumpCard(*candidate);

	return candidate;
}


bool GameLogic::toss(int playerSeatPosition, int tablePosition, uint32_t cardId)
{
	requireState();

	auto player = statePtr->getPlayerDataBySeatPosition(playerSeatPosition);
	auto card = player->takeCardById(cardId);

	if (card == std::nullopt || statePtr->cardsOnTablePositionSize(tablePosition) > 0)
		return false;


	statePtr->addCardToTable(tablePosition, std::move(card.value()));
	return true;
}

bool GameLogic::beat(int playerSeatPosition, int tablePosition, uint32_t cardId)
{
	requireState();

	auto player = statePtr->getPlayerDataBySeatPosition(playerSeatPosition);
	auto card = player->takeCardById(cardId);

	if (card == std::nullopt || statePtr->cardsOnTablePositionSize(tablePosition) != 1)
		return false;


	statePtr->addCardToTable(tablePosition, std::move(card.value()));
	return true;
}


void GameLogic::moveTableCardsToBoneyard()
{
	requireState();

	for (int i = 0; i < statePtr->attackerCardsOnTableSize(); ++i)
	{
		for (int j = statePtr->cardsOnTablePositionSize(i) - 1; j >= 0; --j)
		{
			statePtr->addCardToBoneyard(statePtr->removeCardFromTablePosition(i, j));
		}
	}
}


void GameLogic::moveTableCardsToHand(int seatPosition)
{
	requireState();

	for (int i = 0; i < statePtr->attackerCardsOnTableSize(); ++i)
	{
		for (int j = statePtr->cardsOnTablePositionSize(i) - 1; j >= 0; --j)
		{
			statePtr->addCardToPlayer(seatPosition, statePtr->removeCardFromTablePosition(i, j));
		}
	}
}