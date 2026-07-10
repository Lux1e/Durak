#pragma once
#include "../states/server/GameState.h"
#include "CardData.h"
#include <optional>
#include <memory>



class GameLogic final
{
public:
	GameLogic() : statePtr(nullptr) {}
	GameLogic(GameState& gameState) : statePtr(&gameState) {}


	void shuffleDeck(); //to do

	uint32_t dealOneCard(int playerSeatPosition);

	uint32_t identifyTrumpCard();
	std::optional<std::pair<int, uint32_t>> identifyFirstMove();

	bool toss(int playerSeatPosition, int tablePosition, uint32_t cardId);
	bool beat(int playerSeatPosition, int tablePosition, uint32_t cardId);

	void moveTableCardsToBoneyard();
	void moveTableCardsToHand(int seatPosition);

private:
	GameState* statePtr;

	void requireState() const;
};