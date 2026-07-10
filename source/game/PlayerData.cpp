#include "PlayerData.h"
#include <stdexcept>
#include "../core/DebugLog.h"



const PlayerProfile& PlayerData::getPlayerProfile() const
{
	return playerProfile;
}


uint32_t PlayerData::getId() const
{
	return playerProfile.getId();
}


std::vector<uint32_t> PlayerData::getHandIds() const
{
	std::vector<uint32_t> ids;
	for (const auto& card : hand)
		ids.emplace_back(card.getId());

	return ids;
}

const std::vector<CardData>& PlayerData::getHand() const
{
	return hand;
}

void PlayerData::addCard(CardData&& card)
{
	hand.push_back(std::move(card));
}

std::optional<CardData> PlayerData::takeCardById(uint32_t cardId)
{
	for (auto it = hand.begin(); it != hand.end(); ++it)
	{
		if (it->getId() == cardId)
		{
			CardData card = std::move(*it);
			hand.erase(it);
			return card;
		}
	}

	DEBUG_LOG("Card not found, cardId =" << cardId);
	return std::nullopt;
}

const CardData* PlayerData::getCardById(uint32_t cardId) const
{
	for (const auto& card : hand)
	{
		if (cardId == card.getId())
			return &card;
	}

	DEBUG_LOG("Card not found, cardId =" << cardId);
	return nullptr;
}


std::optional<CardData> PlayerData::findLowestRankBySuit(CardData::Suit suit) const
{
	std::optional<CardData> candidate = std::nullopt;

	for (const auto& card : hand)
	{
		if (card.getSuit() == suit)
		{
			if (candidate == std::nullopt || card.getRank() < candidate->getRank())
				candidate = card;
		}
	}
	
	return candidate;
}


void PlayerData::setWinner(bool value)
{
	winner = value;
}

bool PlayerData::isWinner() const
{
	return winner;
}


void PlayerData::setLoser(bool value)
{
	loser = value;
}

bool PlayerData::isLoser() const
{
	return loser;
}


void PlayerData::setPassAction(bool value)
{
	passAction = value;
}

bool PlayerData::getPassAction() const
{
	return passAction;
}