#pragma once
#include <vector>
#include <unordered_map>
#include <optional>
#include <memory>
#include "../core/IState.h"
#include "../../core/Constants.h"
#include "../../game/PlayerData.h"
#include "../../game/CardData.h"
#include "../SeatPositionsState.h"



class GameState final : public IState
{
public:
	GameState(SeatPositionsState& seatPositions) : m_seatPositions(seatPositions)
	{
		init(seatPositions);
	}


	enum class Phase : uint8_t
	{
		None = 0,
		WaitingForPlayers = 1,
		StartingGame = 2,
		MainPlayerMove = 3,
		EveryPlayerMove = 4,
		RefillPhase = 5,
		GameOver = 6
	};


	bool hasPlayer(const PlayerProfile& profile);

	void setCurrentPhase(Phase newPhase);
	Phase getCurrentPhase() const;
	int getCurrentPhaseInt() const;

	const std::vector<PlayerData>& getPlayers() const;
	void addCardToPlayer(int seatPosition, CardData&& card);

	const PlayerData* getPlayerDataBySeatPosition(int seatPosition) const;
	PlayerData* getPlayerDataBySeatPosition(int seatPosition);

	int getSeatPositionByPlayerData(const PlayerData& player) const;

	const PlayerData* getPlayerDataByPlayerId(uint32_t playerId) const;
	PlayerData* getPlayerDataByPlayerId(uint32_t playerId);

	const PlayerData* getCurrentMainAttackerPlayerData() const;
	PlayerData* getCurrentMainAttackerPlayerData();

	const PlayerData* getCurrentDefenderPlayerData() const;
	PlayerData* getCurrentDefenderPlayerData();

	CardData takeDeckCard();
	CardData& getCardFromDeckByIndex(int index);
	const std::vector<CardData>& getDeck() const;
	void moveInDeck(int fromIndex, int toIndex);

	void addCardToTable(int tablePosition, CardData&& card);
	CardData removeCardFromTablePosition(int tablePosition, int index);
	std::vector<const CardData*> getTableCards() const;
	const std::vector<CardData>& getTableCardsAtPosition(int tablePosition);

	int cardsOnTableSize() const;
	int cardsOnTablePositionSize(int position) const;
	int attackerCardsOnTableSize() const;
	int defenderCardsOnTableSize() const;

	std::vector<uint32_t> getTableCardsIds() const;
	std::vector<uint32_t> getAttackerTableCardsIds() const;
	std::vector<std::pair<int, uint32_t>> getDefenderTableCardsPositionsAndIds() const;

	void addCardToBoneyard(CardData&& card);
	const std::vector<CardData>& getBoneyard() const;


	void setMainTrumpCardId(uint32_t cardId);
	std::optional<uint32_t> getMainTrumpCardId() const;

	void setTrumpSuit(const CardData::Suit suit);
	std::optional<CardData::Suit> getTrumpSuit() const;

	void setPlayersLowestTrumpCard(std::pair<int, uint32_t> value); // first = playerSeatPosition, second cardId
	std::optional<std::pair<int, uint32_t>> getPlayersLowestTrumpCard() const; // first = playerSeatPosition, second cardId

	void setCurrentAttackerSeatPosition(int value);
	const int getCurrentAttackerSeatPosition() const;

	void setCurrentDefenderSeatPosition(int value);
	const int getCurrentDefenderSeatPosition() const;

	bool isDeckEmpty() const;

	std::optional<std::pair<int, uint32_t>> findPlayerWithLowestTrump();

private:
	SeatPositionsState m_seatPositions;
	Phase _CurrentPhase;

	std::vector<CardData> deck;
	std::vector<std::vector<CardData>> table;
	std::vector<CardData> boneyard;

	std::vector<PlayerData> players;

	int _currentAttackerSeatPosition;
	int _currentDefenderSeatPosition;

	std::optional<uint32_t> mainTrumpCardId;
	std::optional<CardData::Suit> trumpSuit;
	std::optional<std::pair<int, uint32_t>> playersLowestTrumpCard;     //<playerSeatPosition, cardId>


	void init(SeatPositionsState& seatPositions);
	void initPlayerData();
};