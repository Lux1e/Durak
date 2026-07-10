#pragma once
#include "../core/DebugLog.h"
#include "../core/Trackable.h"
#include "Rules.h"
#include "../states/server/GameState.h"


class EventSystem;
class GameLogic;
class Event;
class EventInitiator;



class ClassicRules final : public Rules, public Trackable
{
public:
	ClassicRules(GameState& gameState, GameLogic& logic, EventSystem& serverES) : gameState(gameState), logic(logic), serverES(serverES)
	{
		init();
		subscribeAll();
	}

	void update(float dt) override;

private:
	EventSystem& serverES;

	GameState& gameState;
	GameLogic& logic;


	float timer;

	bool allAttackerPlayersPassAction;
	bool allCardsAreBeaten;
	bool defended;
	bool isLimitAttackerCardsReached;

	int maxCardsInPlayersHandsAtStart;

	float switchingPhaseTimerHelper;
	int dealCardSeatPositionHelper;
	GameState::Phase newPhaseHelper;

	bool extraAttackerTossed;

	std::vector<int> attackersOrder;
	std::vector<int> winCandidatesSeatPosition;
	std::vector<int> realWinnersSeatPosition;
	int loserSeatPosition;

	int countWinnersAwarded;


	//---------------Flags for Update---------------
	//SetNewPhaseUpdateFlag:
	bool preparePhaseChangeDone;

	//StartingGameUpdateFlags:
	bool dealCardsDone;
	bool identifyTrumpCardDone;
	bool identifyFirstMoveDone;

	//refillPhaseUpdateFlags
	bool clearTableDone;
	bool clearTableEventDone;
	bool checkForWinnersAndLoserDone;
	bool winAndLoseEventsDone;


	void init();
	void subscribeAll();

	void resetTimer();

	void startingGameUpdate();
	void mainPlayerMoveUpdate(int playersSize);
	void everyPlayerMoveUpdate();
	void refillPhaseUpdate();

	void recalculateRuleState();

	void preparePhaseChange(GameState::Phase newPhase, float timeDelayBeforeChange);

	void setPassAction(PlayerData& player, bool value);
	bool dealCards(float dealDelayTime);
	void setRoles(int attackerSeatPosition ,int defenderSeatPosition);
	void setNewPhase(GameState::Phase phase);
	void setDefended(bool value);
	void clearTable();

	void checkForWinnersAndLoser();
	bool winAndLoseEvents(float awardingDelayTime);

	void determineNextTurnRoles();
	void resetTurn();


	bool canToss(const GameState::Phase& phase, int playerSeatPosition, uint32_t cardId) const;
	bool canBeat(const GameState::Phase& phase, int playerSeatPosition, int tablePosition, uint32_t cardId) const;


	/*void onTossRequestEvent(TossRequestEvent& event);
	void onBeatRequestEvent(BeatRequestEvent& event);
	void onPassActionFromPlayerEvent(PassActionFromPlayerEvent& event);*/
};