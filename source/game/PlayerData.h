#pragma once
#include <vector>
#include <string>
#include "CardData.h"
#include <optional>
#include "PlayerProfile.h"


class PlayerProfile;


class PlayerData final
{
public:
	PlayerData(PlayerProfile& playerProfile) : playerProfile(playerProfile)
	{
		passAction = false;
		winner = false;
		loser = false;
	}

	const PlayerProfile& getPlayerProfile() const;

	uint32_t getId() const;

	std::vector<uint32_t> getHandIds() const;
	const std::vector<CardData>& getHand() const;
	void addCard(CardData&& card);
	std::optional<CardData> takeCardById(uint32_t cardId);
	const CardData* getCardById(uint32_t cardId) const;

	std::optional<CardData> findLowestRankBySuit(CardData::Suit suit) const;

	void setWinner(bool value);
	bool isWinner() const;

	void setLoser(bool value);
	bool isLoser() const;

	void setPassAction(bool value);
	bool getPassAction() const;

private:
	PlayerProfile& playerProfile;

	std::vector<CardData> hand;

	bool winner;
	bool loser;

	bool passAction;
};